#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class DataSet
{
public:
	vector<VectorXd> readImageFile(string filename, bool isTest = false);
	vector<double> readLabelFile(string filename);
	void renderNumber(VectorXd data, ostream& out = cout);
};
