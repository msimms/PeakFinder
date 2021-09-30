#  MIT License
#
#  Copyright (c) 2018 Michael J Simms. All rights reserved.
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.

import numpy as np

class GraphPoint(object):
    """Represents an x,y point value."""
    
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __repr__(self):
        return "( " + str(self.x) + ", " + str(self.y) + " )"

    def clear(self):
        self.x = 0
        self.y = 0.0

class GraphPeak(object):
    """Represents a peak, consisting of a left trough, peak, and right trough."""

    def __init__(self):
        self.left_trough = GraphPoint(0, 0.0)
        self.peak = GraphPoint(0, 0.0)
        self.right_trough = GraphPoint(0, 0.0)
        self.area = 0.0

    def __repr__(self):
        return "{ " + str(self.left_trough) + ", " + str(self.peak) + ", " + str(self.right_trough) + ", " + str(self.area) + " }"

    def clear(self):
        self.left_trough.clear()
        self.peak.clear()
        self.right_trough.clear()
        self.area = 0.0

def compute_area_from_graph_point_array(data, current_peak):
    """Computes the area of the given peak."""
    current_peak.area = 0.0

    if current_peak.left_trough.x < current_peak.right_trough.x:
        prev_point = data[current_peak.left_trough.x]
        for current_point in data[current_peak.left_trough.x + 1:current_peak.right_trough.x + 1]:
            b = current_point.y + prev_point.y
            current_peak.area = current_peak.area + (0.5 * b)
            prev_point = current_point

def compute_area_from_numeric_array(data, current_peak):
    """Computes the area of the given peak."""
    current_peak.area = 0.0

    if current_peak.left_trough.x < current_peak.right_trough.x:
        prev_point = data[current_peak.left_trough.x]
        for current_point in data[current_peak.left_trough.x + 1:current_peak.right_trough.x + 1]:
            b = current_point + prev_point
            current_peak.area = current_peak.area + (0.5 * b)
            prev_point = current_point

def find_peaks_in_numeric_array_over_threshold(data, threshold):
    """Returns a list of peaks in the given array of numeric values. Only peaks that go above the given threshold will be counted."""
    peaks = []
    current_peak = GraphPeak()

    x = 0

    for y in data:

        if y < threshold:

            # Have we found a peak? If so, add it and start looking for the next one.
            if current_peak.right_trough.x > 0:

                # Still descending
                if y <= current_peak.right_trough.y:
                    current_peak.right_trough.x = x
                    current_peak.right_trough.y = y

                # Rising
                else:
                    compute_area_from_numeric_array(data, current_peak)
                    peaks.append(current_peak)
                    current_peak = GraphPeak()

            # Are we looking for a left trough?
            elif current_peak.left_trough.x == 0:
                current_peak.left_trough.x = x
                current_peak.left_trough.y = y
            
            # If we have a left trough and an existing peak, assume this is the right trough - for now.
            elif (current_peak.peak.x > current_peak.left_trough.x) and (current_peak.left_trough.x > 0):
                current_peak.right_trough.x = x
                current_peak.right_trough.y = y

            else:
                current_peak.left_trough.x = x
                current_peak.left_trough.y = y

        # Left trough is set.
        elif current_peak.left_trough.x > 0:

            # Are we looking for a peak or is this bigger than the current peak?
            if current_peak.peak.x == 0 or y >= current_peak.peak.y:
                current_peak.peak.x = x
                current_peak.peak.y = y
                current_peak.right_trough.x = 0
                current_peak.right_trough.y = 0.0

        # Right trough is set.
        elif current_peak.right_trough.x > 0:
            compute_area_from_numeric_array(data, current_peak)
            peaks.append(current_peak)
            current_peak = GraphPeak()

        # Nothing is set, but the value is above the threshold.
        else:
            current_peak.left_trough.x = x
            current_peak.left_trough.y = y

        x = x + 1

    return peaks

def find_peaks_in_numeric_array_over_stddev(data, sigmas):
    """Returns a list of peaks in the given array of numeric values. Only peaks that go above the given sigma line will be counted."""
    mean = np.mean(data)
    stddev = sigmas * np.std(data)
    threshold = mean + stddev

    peaks = find_peaks_in_numeric_array_over_threshold(data, threshold)
    return peaks
