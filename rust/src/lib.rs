//	MIT License
//
//  Copyright © 2021 Michael J Simms. All rights reserved.
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in all
//	copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//	SOFTWARE.
#[allow(dead_code)]

pub mod peaks;

#[cfg(test)]
mod tests {
    use std::fs::File;

    extern crate csv;

    struct AccelerometerData {
        ts: Vec<f64>,
        x: Vec<f64>,
        y: Vec<f64>,
        z: Vec<f64>,
    }

    impl AccelerometerData {
        pub fn new() -> AccelerometerData {
            AccelerometerData { ts: Vec::new(), x: Vec::new(), y: Vec::new(), z: Vec::new() }
        }
    }

    fn read_accelerometer_csv(file_path: &str) -> AccelerometerData {
        let mut accel_vector = AccelerometerData::new();
        let file = match File::open(&file_path) {
            Err(why) => panic!("couldn't open {} {}", file_path, why),
            Ok(file) => file,
        };
        let mut reader = csv::Reader::from_reader(file);

        for record in reader.records() {
            for field in record.iter() {
                let ts: f64 = field[0].parse().unwrap();
                let x: f64 = field[1].parse().unwrap();
                let y: f64 = field[2].parse().unwrap();
                let z: f64 = field[3].parse().unwrap();

                accel_vector.ts.push(ts);
                accel_vector.x.push(x);
                accel_vector.y.push(y);
                accel_vector.z.push(z);
            }
        }
        accel_vector
    }

    #[test]
    fn perform_peak_finding_tests() {
        println!("\nPeak Finding Tests:");
        println!("-------------------");

        let accel_csv_file_name = "../data/pullups.csv";
		let accel_data = read_accelerometer_csv(accel_csv_file_name);

        let x_peaks = crate::peaks::find_peaks_over_threshold(&accel_data.x, 0.0);
        let y_peaks = crate::peaks::find_peaks_over_threshold(&accel_data.y, 0.0);
        let z_peaks = crate::peaks::find_peaks_over_threshold(&accel_data.z, 0.0);

        assert!(x_peaks.len() == 0);
        assert!(y_peaks.len() == 11);

        println!("\nX Peaks:");
        for peak in x_peaks {
            peak.print();
        }
        println!("\nY Peaks:");
        for peak in y_peaks {
            peak.print();
        }
        println!("\nZ Peaks:");
        for peak in z_peaks {
            peak.print();
        }
    }
}
