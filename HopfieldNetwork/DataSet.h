#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <array>

class DataSet
{
private:
	string outFileName = "";
	ofstream ofs;
public:
	void setFileName(string filename);
	vector<VectorXd> readImageFile(string filename);
	vector<double> readLabelFile(string filename);
	void renderNumber(VectorXd data);
};
