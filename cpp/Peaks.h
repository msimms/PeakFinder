// by Michael J. Simms
// Copyright (c) 2018 Michael J. Simms

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#ifndef _PEAKS_
#define _PEAKS_

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>

namespace Peaks
{
	/**
	 * Defines a point. X values are integers. Y values are floating point.
	 */
	class GraphPoint
	{
	public:
		uint64_t x;
		double y;
		
		GraphPoint() { x = 0; y = (double)0.0; }
		GraphPoint(uint64_t newX, double newY) { x = newX; y = newY; }
		GraphPoint(const GraphPoint& rhs) { x = rhs.x; y = rhs.y; }
		
		GraphPoint& operator=(const GraphPoint& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			return *this;
		}

		bool roughlyEqual(double a, double b, double epsilon) const
		{
			double absA = fabs(a);
			double absB = fabs(b);
			return fabs(a - b) <= ( (absA < absB ? absB : absA * epsilon) );
		}

		bool operator==(const GraphPoint& rhs) const
		{
			bool xEqual = roughlyEqual(x, rhs.x, (double)0.0001);
			bool yEqual = roughlyEqual(y, rhs.y, (double)0.0001);
			return xEqual && yEqual;
		}
		
		void clear()
		{
			x = 0;
			y = (double)0.0;
		}
	};

	/**
	 * List of points.
	 */
	typedef std::vector<GraphPoint> GraphLine;

	/**
	 * Defines a peak. A peak is described by three points: a left trough, a peak, and a right trough.
	 */
	class GraphPeak
	{
	public:
		GraphPoint leftTrough;
		GraphPoint peak;
		GraphPoint rightTrough;
		double area;
		
		GraphPeak() { clear(); }
		
		GraphPeak(const GraphPeak& rhs)
		{
			leftTrough = rhs.leftTrough;
			peak = rhs.peak;
			rightTrough = rhs.rightTrough;
			area = rhs.area;
		}
		
		GraphPeak& operator=(const GraphPeak& rhs)
		{
			leftTrough = rhs.leftTrough;
			peak = rhs.peak;
			rightTrough = rhs.rightTrough;
			area = rhs.area;
			return *this;
		}
		
		bool operator==(const GraphPeak& rhs) const
		{
			return (leftTrough == rhs.leftTrough) && (peak == rhs.peak) && (rightTrough == rhs.rightTrough);
		}
		
		bool operator < (const GraphPeak& rhs) const { return (area < rhs.area); }
		bool operator > (const GraphPeak& rhs) const { return (area > rhs.area); }
		
		void clear()
		{
			leftTrough.clear();
			peak.clear();
			rightTrough.clear();
			area = (double)0.0;
		}
	};

	/**
	 * List of peaks.
	 */
	typedef std::vector<GraphPeak> GraphPeakList;

	/**
	 * Collection of peak finding algorithms.
	 */
	class Peaks
	{	
	public:
		/**
		 * Returns a list of all statistically significant peaks in the given waveform.
		 * These are defined as peaks that rise more than one standard deviation above the mean for at least three points on the x axis.
		 */
		static GraphPeakList findPeaksOverThreshold(double* data, size_t dataLen, size_t* numPeaks, double threshold = 0.0);
		static GraphPeakList findPeaksOverStd(double* data, size_t dataLen, size_t* numPeaks, double sigmas = 1.0);
		static GraphPeakList findPeaksOverThreshold(const std::vector<double>& data, double threshold = 0.0);
		static GraphPeakList findPeaksOverStd(const std::vector<double>& data, double sigmas = 1.0);
		static GraphPeakList findPeaksOverThreshold(const GraphLine& data, double threshold = 0.0);
		static GraphPeakList findPeaksOverStd(const GraphLine& data, double sigmas = 1.0);

	private:
		static double average(const double* data, size_t numPoints);
    	static double average(const std::vector<double>& data);
		static double average(const GraphLine& data);

		static double variance(const double* data, size_t numPoints, double mean);
		static double variance(const std::vector<double>& data, double mean);
		static double variance(const GraphLine& data, double mean);

		static double standardDeviation(const double* data, size_t numPoints, double mean);
		static double standardDeviation(const std::vector<double>& data, double mean);
		static double standardDeviation(const GraphLine& data, double mean);

		static void computeArea(double* data, size_t dataLen, GraphPeak& currentPeak);
		static void computeArea(const std::vector<double>& data, GraphPeak& currentPeak);
		static void computeArea(const GraphLine& data, GraphPeak& currentPeak);
	};
}

#endif
