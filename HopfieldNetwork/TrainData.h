#pragma once
#include "DataSet.h"

class TrainData :
	public DataSet
{
public:
	string rawFileName = "rawPatternData.csv"; //出力ファイル名
	string prettyFileName = "prettyPatternData.csv"; //出力ファイル名
	string imagePath;
	string labelPath;
	vector<VectorXd> images;
	vector<double> labels;
	array<VectorXd, 10> patterns;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	TrainData(string imagePath = TRAIN_IMAGE_PATH, string labelPath = TRAIN_LABEL_PATH);
	void load();
	void calc_average_numeric();
	void save_patterns();
};
