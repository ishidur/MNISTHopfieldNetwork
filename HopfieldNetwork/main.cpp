// HopfieldNetwork.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "TrainData.h"
#include "TestData.h"
#include <fstream>

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

//TestData testData;
array<VectorXd, 10> patternSet;
MatrixXd weightMtrx(PIXEL,PIXEL);

vector<string> split(const string& str, char sep)
{
	vector<string> v;
	stringstream ss(str);
	string buffer;
	while (getline(ss, buffer, sep))
	{
		v.push_back(buffer);
	}
	return v;
}

void makePatternSet()
{
	TrainData trainData;
	trainData.load();
	cout << "data loaded successfully!" << endl;
	trainData.calcAverageNumeric();
	trainData.savePatterns();
	patternSet = trainData.patterns;
}

void renderNum(VectorXd data, ostream& out = cout)
{
	int n = int(sqrt(data.size()));
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			int a = int(data[i * n + j]);
			out << a;
			if (j < n - 1)
			{
				out << ",";
			}
		}
		out << endl;
	}
}

void outMatrix(MatrixXd mtrx, ostream& out = cout)
{
	for (int i = 0; i < mtrx.rows(); ++i)
	{
		for (int j = 0; j < mtrx.cols(); ++j)
		{
			out << mtrx(i, j);
			if (j < mtrx.rows() - 1)
			{
				out << ",";
			}
		}
		out << endl;
	}
}

MatrixXd calcQMatrix()
{
	cout << "calculating q matrix..." << endl;
	int lim = patternSet.size();
	MatrixXd result = MatrixXd::Zero(lim, lim);
	for (int i = 0; i < lim; ++i)
	{
		for (int j = 0; j < lim; ++j)
		{
			result(i, j) = patternSet[i].dot(patternSet[j]) / double(PIXEL);
		}
	}
	cout << endl << "Done." << endl;
	return result.inverse();
}

void calcWeightMatrix()
{
	cout << "calculating weight matrix..." << endl;
	MatrixXd qMtrxInv = calcQMatrix();
	string progress = "";
	int n = 0;
	int lim = PIXEL * PIXEL;
	for (int i = 0; i < PIXEL; ++i)
	{
		for (int j = 0; j < PIXEL; ++j)
		{
			n++;

			double status = double(n * 100.0 / (lim));
			if (progress.size() < int(status) / 5)
			{
				progress += "#";
			}
			cout << "progress: " << setw(4) << right << fixed << setprecision(1) << (status) << "% " << progress << "\r" << flush;
			weightMtrx(i, j) = 0.0;

			for (int k = 0; k < patternSet.size(); ++k)
			{
				for (int l = k; l < patternSet.size(); ++l)
				{
					weightMtrx(i, j) += double(patternSet[k][i]) * qMtrxInv(k, l) * double(patternSet[l][j]) / double(PIXEL);
				}
			}
		}
	}
	cout << endl << "Done." << endl;
	ofstream ofs(WEIGHT_MATRIX_FILENAME);
	outMatrix(weightMtrx, ofs);
	ofs.close();
}

void loadWeightMtrxSet()
{
	ifstream ifs(WEIGHT_MATRIX_PATH);
	if (!ifs)
	{
		cout << "weight matirx file is not found" << endl;
		cout << "create weight matirx file..." << endl;
		calcWeightMatrix();
	}
	else
	{
		cout << "loading from weight matirx file" << endl;

		//csvファイルを1行ずつ読み込む
		string str;
		string progress = "";

		int rows = 0;
		while (getline(ifs, str))
		{
			if (rows >= weightMtrx.rows())
			{
				break;
			}
			double status = double(rows * 100.0 / (weightMtrx.rows() - 1));
			if (progress.size() < int(status) / 5)
			{
				progress += "#";
			}
			cout << "progress: " << setw(4) << right << fixed << setprecision(1) << (status) << "% " << progress << "\r" << flush;
			vector<basic_string<char>> p = split(str, ',');
			for (int i = 0; i < p.size(); ++i)
			{
				weightMtrx(rows, i) = stoi(p[i]);
			}
			rows++;
		}
		cout << endl << "Done." << endl;
	}
	cout << weightMtrx(0, 0) << ", " << weightMtrx(0, PIXEL - 1) << ", " << weightMtrx(PIXEL - 1, 0) << ", " << weightMtrx(PIXEL - 1, PIXEL - 1) << endl;
}

void loadPatternSet()
{
	ifstream ifs(PATTERN_PATH);
	if (!ifs)
	{
		cout << "pattern file is not found" << endl;
		cout << "create patttern file..." << endl;
		makePatternSet();
	}
	else
	{
		cout << "loading from pattern file" << endl;
		//csvファイルを1行ずつ読み込む
		string str;
		while (getline(ifs, str))
		{
			vector<basic_string<char>> p = split(str, ',');
			int index = stoi(p[0]);
			p.erase(p.begin());
			cout << index << "; " << p.size() << " pixels" << endl;
			VectorXd tmpPattern = VectorXd::Zero(p.size());
			for (int i = 0; i < p.size(); ++i)
			{
				tmpPattern[i] = stoi(p[i]);
			}
			patternSet[index] = tmpPattern;
		}
		cout << "Done." << endl;
	}
	//	for (int i = 0; i < 10; ++i)
	//	{
	//		renderNum(patternSet[i]);
	//		cout << endl;
	//	}
}

VectorXd updateVector(VectorXd vctr, int index)
{
	VectorXd result = vctr;
	//calculate input
	double inputVal = weightMtrx.col(index).dot(vctr);
	//ignite validation
	int nextVal = 1;
	if (inputVal < 0.0)
	{
		nextVal = -1;
	}
	//update index value
	result[index] = nextVal;
	return result;
}

void recall(VectorXd input, int time = RECALL_TIME)
{
	VectorXd result = input;
	for (int i = 0; i < time; ++i)
	{
		//		pick one
		int n = rand() % input.size();
		result = updateVector(result, n);
	}
	ofstream ofs("out.csv");
	renderNum(result, ofs);
	ofs.close();
}

int main()
{
	loadPatternSet();
	loadWeightMtrxSet();
	recall(patternSet[1]);
	return 0;
}
