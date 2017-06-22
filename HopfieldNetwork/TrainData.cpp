#include "stdafx.h"
#include "TrainData.h"

TrainData::TrainData(string _imagePath, string _labelPath)
{
	imagePath = _imagePath;
	labelPath = _labelPath;
}

void TrainData::setWrite()
{
	setFileName(outFileName);
}


void TrainData::load()
{
	images = readImageFile(imagePath);
	labels = readLabelFile(labelPath);
}

void TrainData::calcAverageNumeric()
{
	array<VectorXd, 10> patterns;
	patterns.fill(VectorXd::Zero(PIXEL));
	int counts[10] = { 0 };
	for (int i = 0; i < images.size(); ++i)
	{
		patterns[int(labels[i])] += images[i];
		counts[int(labels[i])] += 1;
	}

	for (int j = 0; j < 10; ++j)
	{
		if (counts[j] != 0)
		{
			patterns[j] /= counts[j];
		}
		renderNumber(patterns[j]);
		cout << endl;
	}
}