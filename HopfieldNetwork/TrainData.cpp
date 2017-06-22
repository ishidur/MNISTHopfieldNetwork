#include "stdafx.h"
#include "TrainData.h"

TrainData::TrainData(string _imagePath, string _labelPath)
{
	imagePath = _imagePath;
	labelPath = _labelPath;
}

void TrainData::load()
{
	images = readImageFile(imagePath);
	labels = readLabelFile(labelPath);
}

void TrainData::calcAverageNumeric()
{
	ofstream ofs(prettyFileName);
	array<VectorXd, 10> _patterns;
	_patterns.fill(VectorXd::Zero(PIXEL));
	int counts[10] = {0};
	for (int i = 0; i < images.size(); ++i)
	{
		_patterns[int(labels[i])] += images[i];
		counts[int(labels[i])] += 1;
	}

	for (int j = 0; j < 10; ++j)
	{
		if (counts[j] != 0)
		{
			_patterns[j] /= counts[j];
		}
		renderNumber(_patterns[j], ofs);
		ofs << endl;
	}
	ofs.close();
	cout << "write file: " << prettyFileName << " successfully!" << endl;
	patterns = _patterns;
}

void outputPattern(VectorXd v, ostream& out = cout)
{
	int n = v.size();
	for (int i = 0; i < n; ++i)
	{
		int a = int(v[i] / 255 + 0.7);
		out << a;
		if (i < n - 1)
		{
			out << ",";
		}
	}
	out << endl;
}

void TrainData::savePatterns()
{
	if (patterns[0].size() == 0)
	{
		return;
	}
	ofstream ofs(rawFileName);

	for (int i = 0; i < 10; ++i)
	{
		ofs << i << endl;
		outputPattern(patterns[i], ofs);
	}
	ofs.close();
	cout << "write file: " << rawFileName << " successfully!" << endl;
}
