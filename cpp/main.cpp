// by Michael J. Simms
// Copyright (c) 2021 Michael J. Simms

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

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Peaks.h"

typedef std::vector<double> NumVec;

std::vector<NumVec> readThreeAxisDataFromCsv(const std::string& fileName)
{
	std::vector<NumVec> columns;
	std::vector<double> tsList, xList, yList, zList;

	std::string line;
	std::ifstream infile(fileName);

	while (std::getline(infile, line, '\n'))
	{
		std::istringstream iss(line);
		std::string value;
		uint64_t ts = 0;
		double x = 0.0, y = 0.0, z = 0.0;

		iss >> ts;
		iss.ignore(256, ',');
		iss >> x;
		iss.ignore(256, ',');
		iss >> y;
		iss.ignore(256, ',');
		iss >> z;

		tsList.push_back(ts);
		xList.push_back(x);
		yList.push_back(y);
		zList.push_back(z);
	}

	columns.push_back(tsList);
	columns.push_back(xList);
	columns.push_back(yList);
	columns.push_back(zList);
	return columns;
}

void peakFindingTests(const std::vector<NumVec>& csvData)
{
	auto csvIter = csvData.begin();
	++csvIter; // Skip over the timestamp column

	uint16_t axisCount = 0;
	for (; csvIter != csvData.end(); ++csvIter)
	{
		std::cout << "Axis " << ++axisCount << ":" << std::endl;

		const NumVec& columnData = (*csvIter);
		uint64_t peakCount = 0;
		std::vector<Peaks::GraphPeak> peaks = Peaks::Peaks::findPeaksOverStd(columnData, (double)1.5);
		for (auto peakIter = peaks.begin(); peakIter != peaks.end(); ++peakIter)
		{
			Peaks::GraphPeak& peak = (*peakIter);
			std::cout << "Peak " << ++peakCount << ": { " << peak.leftTrough.x << ", " << peak.peak.x << ", " << peak.rightTrough.x << ", " << peak.area << " }" << std::endl;
		}
		std::cout << std::endl;
	}
}

int main(int argc, const char * argv[])
{
	const std::string OPTION_CSV_FILE = "--csv";

	std::string csvFileName;
	
	for (size_t i = 1; i < argc; ++i)
	{
		if ((OPTION_CSV_FILE.compare(argv[i]) == 0) && (i + 1 < argc))
		{
			csvFileName = argv[++i];
		}
	}

	if (csvFileName.length() > 0)
	{
		std::vector<NumVec> csvData = readThreeAxisDataFromCsv(csvFileName);
		std::cout << std::endl;
	}

	return 0;
}
