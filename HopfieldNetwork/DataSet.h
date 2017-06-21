#pragma once
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class DataSet
{
public:
	vector<vector<double>> trainingData;
	vector<double> trainingLabel;
	vector<vector<double>> readTrainingFile(string filename);
	vector<double> readLabelFile(string filename);
};
