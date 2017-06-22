#include "stdafx.h"
#include "TestData.h"

TestData::TestData(string _imagePath, string _labelPath)
{
	imagePath = _imagePath;
	labelPath = _labelPath;
}

void TestData::setWrite()
{
	setFileName(outFileName);
}

void TestData::load()
{
	images = readImageFile(imagePath);
	labels = readLabelFile(labelPath);
}
