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

#include "Peaks.h"

#include <string.h>
#include <math.h>

namespace Peaks
{
	// Computes the area of the given peak.
	void Peaks::computeArea(double* data, size_t dataLen, GraphPeak& currentPeak)
	{
		currentPeak.area = (double)0.0;
		
		if (currentPeak.leftTrough.x < currentPeak.rightTrough.x)
		{
			for (size_t index = (size_t)currentPeak.leftTrough.x + 1; index <= currentPeak.rightTrough.x; ++index)
			{
				double b = data[index] + data[index - 1];
				currentPeak.area += ((double)0.5 * b);
			}
		}
	}

	// Returns a list of peaks in the given array of numeric values. Only peaks that go above the given threshold will be counted.
	GraphPeakList Peaks::findPeaksOverThreshold(double* data, size_t dataLen, size_t* numPeaks, double threshold)
	{
		std::vector<GraphPeak> peaks;
		GraphPeak currentPeak;

		for (size_t x = 0; x < dataLen; ++x)
		{
			double y = data[x];
			
			if (y < threshold)
			{
				// Have we found a peak? If so, add it and start looking for the next one.
				if (currentPeak.rightTrough.x > 0)
				{
					// Still descending
					if (y <= currentPeak.rightTrough.y)
					{
						currentPeak.rightTrough.x = x;
						currentPeak.rightTrough.y = y;
					}

					// Rising
					else
					{
						Peaks::computeArea(data, dataLen, currentPeak);
						peaks.push_back(currentPeak);
						currentPeak.clear();
					}
				}
				
				// Are we looking for a left trough?
				else if (currentPeak.leftTrough.x == 0)
				{
					currentPeak.leftTrough.x = x;
					currentPeak.leftTrough.y = y;
				}
				
				// If we have a left trough and an existing peak, assume this is the right trough - for now.
				else if ((currentPeak.peak.x > currentPeak.leftTrough.x) && (currentPeak.leftTrough.x > 0))
				{
					currentPeak.rightTrough.x = x;
					currentPeak.rightTrough.y = y;
				}
				else
				{
					currentPeak.leftTrough.x = x;
					currentPeak.leftTrough.y = y;
				}
			}
			else if (currentPeak.leftTrough.x > 0) // Left trough is set.
			{
				// Are we looking for a peak or is this bigger than the current peak?
				if (currentPeak.peak.x == 0 || y >= currentPeak.peak.y)
				{
					currentPeak.peak.x = x;
					currentPeak.peak.y = y;
				}
			}
			else if (currentPeak.rightTrough.x > 0) // Right trough is set.
			{
				Peaks::computeArea(data, dataLen, currentPeak);
				peaks.push_back(currentPeak);
				currentPeak.clear();
			}
			else // Nothing is set, but the value is above the threshold.
			{
				currentPeak.leftTrough.x = x;
				currentPeak.leftTrough.y = y;
			}
		}
		
		return peaks;
	}

	// Returns a list of peaks in the given array of numeric values. Only peaks that go above the given sigma line will be counted.
	GraphPeakList Peaks::findPeaksOverStd(double* data, size_t dataLen, size_t* numPeaks, double sigmas)
	{		
		double mean = Peaks::average(data, dataLen);
		double stddev = sigmas * Peaks::standardDeviation(data, dataLen, mean);
		double threshold = mean + stddev;
		return Peaks::findPeaksOverThreshold(data, dataLen, numPeaks, threshold);
	}

	// Computes the area of the given peak.
	void Peaks::computeArea(const std::vector<double>& data, GraphPeak& currentPeak)
	{
		currentPeak.area = (double)0.0;
		
		if (currentPeak.leftTrough.x < currentPeak.rightTrough.x)
		{
			for (size_t index = (size_t)currentPeak.leftTrough.x + 1; index <= currentPeak.rightTrough.x; ++index)
			{
				double b = data.at(index) + data.at(index - 1);
				currentPeak.area += ((double)0.5 * b);
			}
		}
	}

	// Returns a list of peaks in the given array of numeric values. Only peaks that go above the given threshold will be counted.
	GraphPeakList Peaks::findPeaksOverThreshold(const std::vector<double>& data, double threshold)
	{
		std::vector<GraphPeak> peaks;
		GraphPeak currentPeak;

		uint64_t x = 0;

		for (auto iter = data.begin(); iter < data.end(); ++iter, ++x)
		{
			double y = *iter;

			if (y < threshold)
			{
				// Have we found a peak? If so, add it and start looking for the next one.
				if (currentPeak.rightTrough.x > 0)
				{
					// Still descending
					if (y <= currentPeak.rightTrough.y)
					{
						currentPeak.rightTrough.x = x;
						currentPeak.rightTrough.y = y;
					}

					// Rising
					else
					{
						Peaks::computeArea(data, currentPeak);
						peaks.push_back(currentPeak);
						currentPeak.clear();
					}
				}

				// Are we looking for a left trough?
				else if (currentPeak.leftTrough.x == 0)
				{
					currentPeak.leftTrough.x = x;
					currentPeak.leftTrough.y = y;
				}

				// If we have a left trough and an existing peak, assume this is the right trough - for now.
				else if ((currentPeak.peak.x > currentPeak.leftTrough.x) && (currentPeak.leftTrough.x > 0))
				{
					currentPeak.rightTrough.x = x;
					currentPeak.rightTrough.y = y;
				}
				else
				{
					currentPeak.leftTrough.x = x;
					currentPeak.leftTrough.y = y;
				}
			}
			else if (currentPeak.leftTrough.x > 0) // Left trough is set.
			{
				// Are we looking for a peak or is this bigger than the current peak?
				if (currentPeak.peak.x == 0 || y >= currentPeak.peak.y)
				{
					currentPeak.peak.x = x;
					currentPeak.peak.y = y;
				}
			}
			else if (currentPeak.rightTrough.x > 0) // Right trough is set.
			{
				Peaks::computeArea(data, currentPeak);
				peaks.push_back(currentPeak);
				currentPeak.clear();
			}
			else // Nothing is set, but the value is above the threshold.
			{
				currentPeak.leftTrough.x = x;
				currentPeak.leftTrough.y = y;
			}
		}
		
		return peaks;
	}

	// Returns a list of peaks in the given array of numeric values. Only peaks that go above the given sigma line will be counted.
	GraphPeakList Peaks::findPeaksOverStd(const std::vector<double>& data, double sigmas)
	{
		double mean = Peaks::average(data);
		double stddev = sigmas * Peaks::standardDeviation(data, mean);
		double threshold = mean + stddev;
		return Peaks::findPeaksOverThreshold(data, threshold);
	}

	double Peaks::average(const double* data, size_t numPoints)
	{
		double sum = 0;

		for (auto index = 0; index < numPoints; index++)
			sum = sum + data[index];
		return sum / (double)numPoints;
	}

	double Peaks::average(const std::vector<double>& data)
	{
		double sum = 0;

		for (auto iter = data.begin(); iter != data.end(); ++iter)
			sum = sum + (*iter);
		return sum / (double)data.size();
	}

	double Peaks::average(const GraphLine& data)
	{
		double sum = (double)0.0;
		
		for (auto iter = data.begin(); iter != data.end(); ++iter)
			sum = sum + (*iter).y;
		return sum / (double)data.size();
	}

	double Peaks::variance(const double* data, size_t numPoints, double mean)
	{
		double numerator = (double)0.0;

		for (auto index = 0; index < numPoints; index++)
			numerator = numerator + ((data[index] - mean) * (data[index] - mean));
		return numerator / (double)(numPoints - 1);
	}

	double Peaks::variance(const std::vector<double>& data, double mean)
	{
		double numerator = (double)0.0;

		for (auto iter = data.begin(); iter != data.end(); ++iter)
			numerator = numerator + ((*iter - mean) * (*iter - mean));
		return numerator / (double)(data.size() - 1);
	}

	double Peaks::variance(const GraphLine& data, double mean)
	{
		double numerator = (double)0.0;
		
		for (auto iter = data.begin(); iter != data.end(); ++iter)
			numerator = numerator + (((*iter).y - mean) * ((*iter).y - mean));
		return numerator / (double)(data.size() - 1);
	}

	double Peaks::standardDeviation(const double* data, size_t numPoints, double mean)
	{
		double var = variance(data, numPoints, mean);
		return sqrt(var);
	}

	double Peaks::standardDeviation(const std::vector<double>& data, double mean)
	{
		double var = variance(data, mean);
		return sqrt(var);
	}

	double Peaks::standardDeviation(const GraphLine& data, double mean)
	{
		double var = variance(data, mean);
		return sqrt(var);
	}

	// Computes the area of the given peak.
	void Peaks::computeArea(const GraphLine& data, GraphPeak& currentPeak)
	{
		currentPeak.area = (double)0.0;
		
		if (currentPeak.leftTrough.x < currentPeak.rightTrough.x)
		{
			auto pointIter = std::find(data.begin(), data.end(), currentPeak.leftTrough);

			if (pointIter != data.end())
			{
				GraphPoint prevPoint = (*pointIter);
				++pointIter;

				for (; pointIter != data.end(); ++pointIter)
				{
					double b = (*pointIter).y + prevPoint.y;
					currentPeak.area += ((double)0.5 * b);
					prevPoint = (*pointIter);
				}
			}
		}
	}

	// Returns a list of peaks in the given array of graph points. Only peaks that go above the given threshold will be counted.
	GraphPeakList Peaks::findPeaksOverThreshold(const GraphLine& data, double threshold)
	{
		std::vector<GraphPeak> peaks;
		GraphPeak currentPeak;

		for (auto iter = data.begin(); iter < data.end(); ++iter)
		{
			const GraphPoint& pt = *iter;

			if (pt.y < threshold)
			{
				// Have we found a peak? If so, add it and start looking for the next one.
				if (currentPeak.rightTrough.x > 0) // Right trough is set
				{
					// Still descending
					if (pt.y <= currentPeak.rightTrough.y)
					{
						currentPeak.rightTrough.x = pt.x;
						currentPeak.rightTrough.y = pt.y;
					}

					// Rising
					else
					{
						Peaks::computeArea(data, currentPeak);
						peaks.push_back(currentPeak);
						currentPeak.clear();
					}
				}

				// Are we looking for a left trough?
				else if (currentPeak.leftTrough.x == 0) // Left trough is not set.
				{
					currentPeak.leftTrough = pt;
				}

				// If we have a left trough and an existing peak, assume this is the right trough - for now.
				else if ((currentPeak.peak.x > currentPeak.leftTrough.x) && (currentPeak.leftTrough.x > 0))
				{
					currentPeak.rightTrough = pt;
				}
				else
				{
					currentPeak.leftTrough = pt;
				}
			}
			else if (currentPeak.leftTrough.x > 0) // Left trough is set.
			{
				// Are we looking for a peak or is this bigger than the current peak, making it the real peak?
				if (currentPeak.peak.x == 0 || pt.y >= currentPeak.peak.y)
				{
					currentPeak.peak = pt;
				}
			}
			else if (currentPeak.rightTrough.x > 0) // Right trough is set.
			{
				Peaks::computeArea(data, currentPeak);
				peaks.push_back(currentPeak);
				currentPeak.clear();
			}
			else // Nothing is set, but the value is above the threshold.
			{
				currentPeak.leftTrough = pt;
			}
		}
		
		return peaks;
	}

	// Returns a list of peaks in the given array of graph points. Only peaks that go above the given sigma line will be counted.
	GraphPeakList Peaks::findPeaksOverStd(const GraphLine& data, double sigmas)
	{
		double mean = average(data);
		double stddev = sigmas * standardDeviation(data, mean);
		double threshold = mean + stddev;
		return Peaks::findPeaksOverThreshold(data, threshold);
	}
}
