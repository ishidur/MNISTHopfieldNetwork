#pragma once
#include "DataSet.h"

class TrainData :
	public DataSet
{
public:
	string imagePath;
	string labelPath;
	vector<VectorXd> images;
	vector<double> labels;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	TrainData(string imagePath = TRAIN_IMAGE_PATH, string labelPath = TRAIN_LABEL_PATH);
	void load();
};
