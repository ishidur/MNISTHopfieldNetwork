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
	array<VectorXd, 10> _patterns;
	_patterns.fill(VectorXd::Zero(PIXEL));
	int counts[10] = {0};
	cout << "caluculating average pattern..." << endl;
	string progress = "";

	for (int i = 0; i < images.size(); ++i)
	{
		double status = i / (images.size() - 1) * 100.0;
		if (int(status) % 10 == 9)
		{
			progress += "#";
		}
		cout << progress << "\r" << flush;
		_patterns[int(labels[i])] += images[i];
		counts[int(labels[i])] += 1;
	}

	ofstream ofs(prettyFileName);

	cout << "writing created pattern pretty style..." << endl;
	progress = "";
	for (int j = 0; j < 10; ++j)
	{
		progress += "#";
		cout << progress << "\r" << flush;
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

VectorXd outputPattern(VectorXd v, ostream& out = cout)
{
	int n = v.size();
	VectorXd o(v.size());
	for (int i = 0; i < n; ++i)
	{
		int a = int(v[i] / 255 + 0.7);
		out << a;
		if (i < n - 1)
		{
			out << ",";
		}
		o[i] = a;
	}
	out << endl;
	return o;
}

void TrainData::savePatterns()
{
	if (patterns[0].size() == 0)
	{
		return;
	}
	ofstream ofs(rawFileName);
	cout << "writing pattern data file..." << endl;
	string progress = "";
	for (int i = 0; i < 10; ++i)
	{
		progress += "#";
		cout << progress << "\r" << flush;
		ofs << i << ",";
		patterns[i] = outputPattern(patterns[i], ofs);
	}
	ofs.close();
	cout << "write file: " << rawFileName << " successfully!" << endl;
}
