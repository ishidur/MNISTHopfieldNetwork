#include "stdafx.h"
#include "TestData.h"

TestData::TestData(string _imagePath, string _labelPath)
{
	imagePath = _imagePath;
	labelPath = _labelPath;
}

void TestData::load()
{
	images = read_image_file(imagePath, true);
	labels = read_label_file(labelPath);
}
