#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "Eigen/Core"

using namespace std;
using namespace Eigen;

class DataSet
{
public:
	vector<VectorXd> data;
	vector<double> label;
	vector<VectorXd> readTrainingFile(string filename);
	vector<double> readLabelFile(string filename);
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
