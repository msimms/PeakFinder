# PeakFinder

A library for extracting peaks from line data with implementations in multiple languages.  The line data could be from accelerometer data, spectral data, stock price history, almost anything.

Here's a graphical example of how this works, indentifying the peaks and troughs in the given line data. The blue line is the original data and the green line shows the peaks identified by the peak finding algorithm:

![Example](https://github.com/msimms/PeakFinder/blob/main/docs/example.png?raw=true)

## Installation
### C++

Copy the files `Peaks.cpp` and `Peaks.h` into your project. Look at `main.cpp` for an example of how to use the peak finding class.
* `GraphPeakList Peaks::findPeaksOverThreshold();`
* `GraphPeakList Peaks::findPeaksOverStd();`

### Julia

Copy the file `Peaks.jl` into your project. Look at `PeakFinder.jl` for an example of how to use the peak finding class.
* `find_peaks_over_threshold(data::Array{GraphPoint}, threshold)`
* `find_peaks_over_stddev(data::Array{GraphPoint}, sigmas)`

### Python

Copy the file `peaks.py` into your project, though I will eventually create a `pip` installable package for it.
* `find_peaks_in_numeric_array_over_threshold(data, threshold)`
* `find_peaks_in_numeric_array_over_stddev(data, sigmas)`

### Rust

Copy the file `peaks.rs` into your project. Look at `lib.rs` for an example. The available functions are:
* `find_peaks_in_numeric_array_over_threshold(data, threshold)`
* `find_peaks_in_numeric_array_over_stddev(data, sigmas)`

## Version History

v.1.0.0 - Initial release.

## License
This library is released under the MIT license, see LICENSE for details.
