#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class DataSet
{
public:
	vector<VectorXd> readImageFile(string filename, bool isBipolar = false);
	vector<double> readLabelFile(string filename);
	VectorXd renderNumber(VectorXd data, ostream& out = cout);
};
