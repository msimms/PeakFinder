# MIT License
#
# Copyright © 2020 Michael J Simms. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

module Peaks

import Statistics
export find_peaks

# Defines a point. X values are integers. Y values are floating point.
mutable struct GraphPoint
    x::UInt
    y::Float64
end

# List of points.
#struct GraphLine <: Array{GraphPoint} end

# Defines a peak. A peak is described by three points: a left trough, a peak, and a right trough.
mutable struct GraphPeak
    left_trough::GraphPoint
    peak::GraphPoint
    right_trough::GraphPoint
    area::Float64
end

# List of points.
#struct GraphPeakList <: Array{GraphPeak} end

function compute_area(data, current_peak::GraphPeak)
    current_peak.area = 0.0
    
    if current_peak.left_trough.x < current_peak.right_trough.x
        for i in current_peak.left_trough.x+1:current_peak.right_trough.x
            b = data[i] + data[i - 1]
            current_peak.area += (0.5 * b)
        end
    end
end

# Returns a list of all statistically significant peaks in the given waveform.
# These are defined as peaks that rise more than one standard deviation above the mean for at least three points on the x axis.

function find_peaks(data::Array{Float64}, sigmas = 1.0)
    peaks = []

    mean = Statistics.mean(data)
    stddev = sigmas * Statistics.std(data)
    threshold = mean + stddev
    current_peak = GraphPeak(GraphPoint(0,0.0), GraphPoint(0,0.0), GraphPoint(0,0.0), 0.0)
    x = 1

    for y in data

        if y < threshold

            # Have we found a peak? If so, add it and start looking for the next one.
            if current_peak.right_trough.x > 0
                compute_area(data, current_peak)
                push!(peaks, current_peak)
                current_peak = GraphPeak(GraphPoint(0,0.0), GraphPoint(0,0.0), GraphPoint(0,0.0), 0.0)

            # Are we looking for a left trough?
            elseif current_peak.left_trough.x == 0
                current_peak.left_trough.x = x
                current_peak.left_trough.y = y

            # If we have a left trough and an existing peak, assume this is the right trough - for now.
            elseif (current_peak.peak.x > current_peak.left_trough.x) && (current_peak.left_trough.x > 0)
                current_peak.right_trough.x = x
                current_peak.right_trough.y = y

            else
                current_peak.left_trough.x = x
                current_peak.left_trough.y = y
            end

        elseif current_peak.left_trough.x > 0 # Left trough is set.

            # Are we looking for a peak or is this bigger than the current peak?
            if current_peak.peak.x == 0 || y >= current_peak.peak.y
                current_peak.peak.x = x
                current_peak.peak.y = y
                current_peak.right_trough.x = 0
                current_peak.right_trough.y = 0.0
            end

        elseif current_peak.right_trough.x > 0 # Right trough is set.
            compute_area(data, current_peak)
            push!(peaks, current_peak)
            current_peak = GraphPeak(GraphPoint(0,0.0), GraphPoint(0,0.0), GraphPoint(0,0.0), 0.0)

        else # Nothing is set, but the value is above the threshold.
            current_peak.left_trough.x = x
            current_peak.left_trough.y = y
        end

        x = x + 1
    end

    peaks
end

function average(data::Array{GraphPoint})
    sum = 0.0

    for item in data
        sum = sum + item.y
    end

    avg = sum / length(data)
    avg
end

function variance(data::Array{GraphPoint}, mean)
    numerator = 0.0

    for item in data
        numerator = numerator + ((item.y - mean) * (item.y - mean))
    end

    variance = numerator / length(data)
    variance
end

function standard_deviation(data::Array{GraphPoint}, mean)
    var = variance(data, mean)
    dev = sqrt(var)
    dev
end

function find_peaks(data::Array{GraphPoint}, sigmas = 1.0)
    peaks = []

    mean = average(data)
    stddev = sigmas * standard_deviation(data, mean)
    threshold = mean + stddev
    current_peak = GraphPeak(GraphPoint(0,0.0), GraphPoint(0,0.0), GraphPoint(0,0.0), 0.0)
    x = 1

    for point in data

        if point.y < threshold

            # Have we found a peak? If so, add it and start looking for the next one.
            if current_peak.right_trough.x > 0
                compute_area(data, current_peak)
                push!(peaks, current_peak)
                current_peak = GraphPeak(GraphPoint(0,0.0), GraphPoint(0,0.0), GraphPoint(0,0.0), 0.0)

            # Are we looking for a left trough?
            elseif current_peak.left_trough.x == 0
                current_peak.left_trough = point

            # If we have a left trough and an existing peak, assume this is the right trough - for now.
            elseif (current_peak.peak.x > current_peak.left_trough.x) && (current_peak.left_trough.x > 0)
                current_peak.right_trough = point

            else
                current_peak.left_trough = point
            end

        elseif current_peak.left_trough.x > 0 # Left trough is set.

            # Are we looking for a peak or is this bigger than the current peak?
            if current_peak.peak.x == 0 || y >= current_peak.peak.y
                current_peak.peak = point
            end

        elseif current_peak.right_trough.x > 0 # Right trough is set.
            compute_area(data, current_peak)
            push!(peaks, current_peak)
            current_peak = GraphPeak(GraphPoint(0,0.0), GraphPoint(0,0.0), GraphPoint(0,0.0), 0.0)

        else # Nothing is set, but the value is above the threshold.
            current_peak.left_trough = point
        end

        x = x + 1
    end

    peaks
end

function find_peaks_of_size(data::Array{GraphPoint}, minpeakarea, sigmas = 1.0)
    peaks = []

    mean = average(data)
    stddev = sigmas * standard_deviation(data, mean)
    threshold = mean + stddev
    current_peak = GraphPeak(GraphPoint(0,0.0), GraphPoint(0,0.0), GraphPoint(0,0.0), 0.0)
    x = 1

    for point in data

        if point.y < threshold

            # Have we found a peak? If so, add it and start looking for the next one.
            if current_peak.right_trough.x > 0
                compute_area(data, current_peak)
                if current_peak.area >= minpeakarea
                    push!(peaks, current_peak)
                end
                current_peak = GraphPeak(GraphPoint(0,0.0), GraphPoint(0,0.0), GraphPoint(0,0.0), 0.0)

            # Are we looking for a left trough?
            elseif current_peak.left_trough.x == 0
                current_peak.left_trough = point

            # If we have a left trough and an existing peak, assume this is the right trough - for now.
            elseif (current_peak.peak.x > current_peak.left_trough.x) && (current_peak.left_trough.x > 0)
                current_peak.right_trough = point

            else
                current_peak.left_trough = point
            end

        elseif current_peak.left_trough.x > 0 # Left trough is set.

            # Are we looking for a peak or is this bigger than the current peak?
            if current_peak.peak.x == 0 || y >= current_peak.peak.y
                current_peak.peak = point
            end

        elseif current_peak.right_trough.x > 0 # Right trough is set.
            compute_area(data, current_peak)
            if current_peak.area >= minpeakarea
                push!(peaks, current_peak)
            end
            current_peak = GraphPeak(GraphPoint(0,0.0), GraphPoint(0,0.0), GraphPoint(0,0.0), 0.0)

        else # Nothing is set, but the value is above the threshold.
            current_peak.left_trough = point
        end

        x = x + 1
    end

    peaks
end

end
