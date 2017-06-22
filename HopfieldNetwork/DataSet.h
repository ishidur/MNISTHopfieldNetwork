#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class DataSet
{
public:
	vector<VectorXd> readImageFile(string filename);
	vector<double> readLabelFile(string filename);
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
