#include "stdafx.h"
#include "TestData.h"

TestData::TestData(string _imagePath, string _labelPath)
{
	imagePath = _imagePath;
	labelPath = _labelPath;
}

void TestData::load()
{
	images = readImageFile(imagePath, true);
	labels = readLabelFile(labelPath);
}
