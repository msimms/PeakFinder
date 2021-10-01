#  MIT License
#
#  Copyright (c) 2021 Michael J Simms. All rights reserved.
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

import argparse
import csv
import os
import peaks
import sys

def read_three_axis_data_from_csv(csv_file_name):
    """Loads the peak data test file. Expected format is timestamp, x, y, z."""
    columns = []
    ts_list = []
    x_list = []
    y_list = []
    z_list = []

    with open(csv_file_name) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        for row in csv_reader:
            ts_list.append(float(row[0])) # timestamp
            x_list.append(float(row[1]))  # x
            y_list.append(float(row[2]))  # y
            z_list.append(float(row[3]))  # z

	columns.append(ts_list)
	columns.append(x_list)
	columns.append(y_list)
	columns.append(z_list)
	return columns

def find_peaks(file_name, threshold):
    """Runs the peak finding algorithm over the data that was read from the csv file."""
    real_path = os.path.realpath(file_name)
    csv_data = read_three_axis_data_from_csv(real_path)

    axis_peaks = {}
    axis_peaks['x'] = peaks.find_peaks_in_numeric_array_over_threshold(csv_data[1], threshold)
    axis_peaks['y'] = peaks.find_peaks_in_numeric_array_over_threshold(csv_data[2], threshold)
    axis_peaks['z'] = peaks.find_peaks_in_numeric_array_over_threshold(csv_data[3], threshold)
    return axis_peaks

def main():
    # Parse the command line options.
    parser = argparse.ArgumentParser()
    parser.add_argument("--csv", default="../data/pullups.csv", help="CSV file in the format of timestamp, x, y, z", required=False)
    parser.add_argument("--threshold", default=0.0, help="Peaks must be above this threshold value to be considered", required=False)

    try:
        args = parser.parse_args()
    except IOError as e:
        parser.error(e)
        sys.exit(1)

    if len(args.csv) > 0:
        axis_peaks = find_peaks(args.csv, args.threshold)

        print("X-Axis Peaks")
        for peak in axis_peaks['x']:
            print(peak)
        print("Y-Axis Peaks")
        for peak in axis_peaks['y']:
            print(peak)
        print("Z-Axis Peaks")
        for peak in axis_peaks['z']:
            print(peak)
    else:
        print("A CSV file was not specified.")

if __name__ == "__main__":
    main()
