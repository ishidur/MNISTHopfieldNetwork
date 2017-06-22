#pragma once
#include "DataSet.h"

class TestData :
	public DataSet
{
public:
	string outFileName = "testData.csv"; //出力ファイル名
	string imagePath;
	string labelPath;
	vector<VectorXd> images;
	vector<double> labels;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	TestData(string imagePath = TEST_IMAGE_PATH, string labelPath = TEST_LABEL_PATH);
	void setWrite();
	void load();
};
