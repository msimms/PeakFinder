#  MIT License
#
#  Copyright (c) 2020 Michael J Simms. All rights reserved.
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

include("Peaks.jl")

using Pkg
Pkg.add("ArgParse")
using ArgParse
Pkg.add("CSV")
using CSV

function read_accelerometer_csv(filename::String)
    data = []
    data = CSV.read(filename)
    ts = data[1]
    x = data[2]
    y = data[3]
    z = data[4]
    ts, x, y, z
end

# Parses the command line arguments
function parse_commandline()
    s = ArgParseSettings()

    @add_arg_table s begin
        "--csv"
            help = "A CSV file with accelerometer data"
            arg_type = String
            default = "../data/pullups.csv"
        "--threshold"
            help = "Peaks must be above this threshold value to be considered"
            arg_type = Number
            default = 0.0
    end

    return parse_args(s)
end

parsed_args = parse_commandline()
ts, x, y, z = read_accelerometer_csv(parsed_args["csv"])

x_peaks = Peaks.find_peaks_over_stddev(x)
y_peaks = Peaks.find_peaks_over_stddev(y)
z_peaks = Peaks.find_peaks_over_stddev(z)

println("X-Axis Peaks")
println(x_peaks)
println("Y-Axis Peaks")
println(y_peaks)
println("Z-Axis Peaks")
println(z_peaks)
