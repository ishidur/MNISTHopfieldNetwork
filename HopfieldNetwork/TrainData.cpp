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
