#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class DataSet
{
public:
	vector<VectorXd> read_image_file(string filename, bool isTest = false);
	vector<double> read_label_file(string filename);
	void render_number(VectorXd data, ostream& out = cout);
};
