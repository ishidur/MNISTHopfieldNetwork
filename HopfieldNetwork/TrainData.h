#pragma once
#include "DataSet.h"

class TrainData :
	public DataSet
{
public:
	string outFileName = "trainData.csv"; //�o�̓t�@�C����
	string prettyFileName = "trainData-pretty.csv"; //�o�̓t�@�C����
	string imagePath;
	string labelPath;
	vector<VectorXd> images;
	vector<double> labels;
	array<VectorXd, 10> patterns;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	TrainData(string imagePath = TRAIN_IMAGE_PATH, string labelPath = TRAIN_LABEL_PATH);
	void load();
	void calcAverageNumeric();
	void savePatterns();
};
