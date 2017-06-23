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

std::vector<std::string> split(const std::string& str, char sep)
{
	std::vector<std::string> v;
	std::stringstream ss(str);
	std::string buffer;
	while (std::getline(ss, buffer, sep))
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

void calcWeightMatrix()
{
	cout << "calculating weight matrix..." << endl;
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
			if (i == j)
			{
				continue;
			}
			for (int k = 0; k < patternSet.size(); k++)
			{
				weightMtrx(i, j) += double(patternSet[k][i]) * double(patternSet[k][j]);
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

int main()
{
	loadPatternSet();
	loadWeightMtrxSet();
	return 0;
}
