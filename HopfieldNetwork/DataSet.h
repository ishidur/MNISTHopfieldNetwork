#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <array>

class DataSet
{
public:
	vector<VectorXd> readImageFile(string filename);
	vector<double> readLabelFile(string filename);
	void renderNumber(VectorXd data, ostream  &out = cout);
};
