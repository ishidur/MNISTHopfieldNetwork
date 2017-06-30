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

#include <ppl.h>
#include <time.h>     // for clock()

TestData testData;
array<VectorXd, 10> patternSet;
array<VectorXd, 10> validationSet;
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
	std::cout << "data loaded successfully!" << endl;
	trainData.calcAverageNumeric();
	trainData.savePatterns();
	patternSet = trainData.patterns;
}

void loadVaildationPatternSet()
{
	ifstream ifs(VALIDATION_DATA_PATH);
	std::cout << "loading from validation file" << endl;
	//csvファイルを1行ずつ読み込む
	string str;
	while (getline(ifs, str))
	{
		vector<basic_string<char>> p = split(str, ',');
		int index = stod(p[0]);
		p.erase(p.begin());
		std::cout << index << "; " << p.size() << " pixels" << endl;
		VectorXd tmpPattern = VectorXd::Zero(p.size());
		for (int i = 0; i < p.size(); ++i)
		{
			tmpPattern[i] = stod(p[i]);
		}
		validationSet[index] = tmpPattern;
	}
	std::cout << "Done." << endl;
	ifs.close();
}

void renderNum(VectorXd data, ostream& out = std::cout)
{
	int n = int(sqrt(data.size()));
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			double a = double(data[i * n + j]);
			out << a;
			if (j < n - 1)
			{
				out << ",";
			}
		}
		out << endl;
	}
}

void outMatrix(MatrixXd mtrx, ostream& out = std::cout)
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
	std::cout << "calculating q matrix..." << endl;
	int lim = patternSet.size();
	MatrixXd result = MatrixXd::Zero(lim, lim);
	for (int i = 0; i < lim; ++i)
	{
		for (int j = 0; j < lim; ++j)
		{
			result(i, j) = double(patternSet[i].dot(patternSet[j])) / double(PIXEL);
		}
	}
	std::cout << endl << "Done." << endl;
	return result.inverse();
}

void calcWeightMatrix()
{
	std::cout << "calculating weight matrix..." << endl;
	MatrixXd qMtrxInv = calcQMatrix();
	int n = 0;
	int lim = PIXEL * PIXEL;
	Concurrency::parallel_for(0, PIXEL, 1, [&n, &lim, &qMtrxInv](int i)
                          {
	                          for (int j = 0; j < PIXEL; ++j)
	                          {
		                          n++;
		                          double status = double(n * 100.0 / (lim - 1));
		                          std::cout << fixed << setprecision(1) << status << "%" << "\r" << flush;
		                          weightMtrx(i, j) = 0.0;
		                          for (int k = 0; k < patternSet.size(); ++k)
		                          {
			                          for (int l = 0; l < patternSet.size(); ++l)
			                          {
				                          weightMtrx(i, j) += double(patternSet[k][i]) * qMtrxInv(k, l) * double(patternSet[l][j]) / double(PIXEL);
			                          }
		                          }
	                          }
                          });
	std::cout << endl << "Done." << endl;
	ofstream ofs(WEIGHT_MATRIX_FILENAME);
	outMatrix(weightMtrx, ofs);
	ofs.close();
}

void loadWeightMtrxSet()
{
	ifstream ifs(WEIGHT_MATRIX_PATH);
	if (!ifs)
	{
		std::cout << "weight matirx file is not found" << endl;
		std::cout << "create weight matirx file..." << endl;
		calcWeightMatrix();
	}
	else
	{
		std::cout << "loading from weight matirx file" << endl;

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
			std::cout << "progress: " << setw(4) << right << fixed << setprecision(1) << (status) << "% " << progress << "\r" << flush;
			vector<basic_string<char>> p = split(str, ',');
			for (int i = 0; i < p.size(); ++i)
			{
				weightMtrx(rows, i) = stod(p[i]);
			}
			rows++;
		}
		std::cout << endl << "Done." << endl;
		ifs.close();
	}
}

void loadPatternSet()
{
	ifstream ifs(PATTERN_PATH);
	if (!ifs)
	{
		std::cout << "pattern file is not found" << endl;
		std::cout << "create patttern file..." << endl;
		makePatternSet();
	}
	else
	{
		std::cout << "loading from pattern file" << endl;
		//csvファイルを1行ずつ読み込む
		string str;
		while (getline(ifs, str))
		{
			vector<basic_string<char>> p = split(str, ',');
			int index = stod(p[0]);
			p.erase(p.begin());
			std::cout << index << "; " << p.size() << " pixels" << endl;
			VectorXd tmpPattern = VectorXd::Zero(p.size());
			for (int i = 0; i < p.size(); ++i)
			{
				tmpPattern[i] = stod(p[i]);
			}
			patternSet[index] = tmpPattern;
		}
		std::cout << "Done." << endl;
		ifs.close();
	}
	//	for (int i = 0; i < 10; ++i)
	//	{
	//		renderNum(patternSet[i]);
	//		std::cout << endl;
	//	}
}

tuple<int, double> verifyPattern(VectorXd input, bool output = false)
{
	array<double, 10> error = {};
	double min;
	int num;
	//	array<VectorXd, 10> answerSet = validationSet;
	array<VectorXd, 10> answerSet = patternSet;
	for (int i = 0; i < answerSet.size(); ++i)
	{
		error[i] = 0.0;

		for (int j = 0; j < input.size(); ++j)
		{
			error[i] += (answerSet[i][j] - input[j]) * (answerSet[i][j] - input[j]);
		}
		error[i] /= double(input.size());
		if (i == 0 || error[i] < min)
		{
			min = error[i];
			num = i;
		}
		if (output)
		{
			std::cout << i << ", error " << scientific << setprecision(2) << error[i] << endl;
		}
	}
	return forward_as_tuple(num, error[num]);
}

double activationFunc(double input)
{
	//	double result = input * ACTIVATION_CONST;
	//	if (result > 1.0)
	//	{
	//		result = 1.0;
	//	}
	//	else if (result < -1.0)
	//	{
	//		result = -1.0;
	//	}
	double result = tanh(ACTIVATION_CONST * input);
	return result;
}

VectorXd updateVector(VectorXd vctr, int index)
{
	VectorXd result = vctr;
	//calculate input
	double inputVal = weightMtrx.col(index).dot(vctr);
	//ignite validation
	double nextVal = activationFunc(inputVal);
	//update index value
	result[index] = nextVal;
	return result;
}

void noiseRecall(VectorXd input, ostream& out = std::cout)
{
	VectorXd result = input;
	for (int i = 0; i < RECALL_TIME; ++i)
	{
		if (i == 0)
		{
			out << "before:" << endl;

			renderNum(result, out);
			out << endl << endl;
		}
		//		pick one
		int n = rand() % input.size();
		result = updateVector(result, n);
	}
	out << "after " << RECALL_TIME << " iterations" << endl;
	double error;
	int num;
	tie(num, error) = verifyPattern(result, true);
	std::cout << "recalled: " << num << ", error " << scientific << setprecision(3) << error << endl;
	renderNum(result, out);
	out << endl << endl;
}

int recallTest(VectorXd input, int ans, ostream& out = std::cout)
{
	VectorXd result = input;
	for (int i = 0; i < RECALL_TIME; ++i)
	{
		//		pick one
		int n = rand() % input.size();
		result = updateVector(result, n);
	}
	int num;
	double error;
	tie(num, error) = verifyPattern(result);
	out << "answer: ," << ans << ",recalled: ," << num << ", error," << scientific << setprecision(3) << error << endl;
	return num;
}

void runNoiseRecallTest()
{
	ofstream ofs("noise.csv");
	for (int i = 0; i < 10; ++i)
	{
		VectorXd testMtrx = patternSet[i];
		//add noise
		for (int j = 0; j < 200; ++j)
		{
			int n = rand() % testMtrx.size();
			testMtrx[n] *= -1;
		}
		noiseRecall(testMtrx, ofs);
	}
	ofs.close();
}

void runTest()
{
	testData.load();
	ofstream ofs("testResult.csv");
	array<int, 10> trial = {};
	array<int, 10> correct = {};
	int n = testData.labels.size();
	int sum = 1;
	//	float progress;
	std::cout << endl;

	Concurrency::parallel_for(0, n, 1, [&sum, &ofs, &trial, &correct](int i)
                          {
	                          std::cout << "progress: " << sum << " / " << testData.labels.size() << " \r" << flush;
	                          int result = recallTest(testData.images[i], testData.labels[i], ofs);
	                          trial[testData.labels[i]] += 1;
	                          if (result == testData.labels[i])
	                          {
		                          correct[testData.labels[i]] += 1;
	                          }
	                          sum++;
                          });

	std::cout << endl << "Done." << endl;
	ofs << endl;
	for (int i = 0; i < 10; ++i)
	{
		ofs << "accuracy," << i << "," << double(correct[i]) / double(trial[i]) * 100.0 << ",%,correct," << correct[i] << ",trial," << trial[i] << endl;
	}
	ofs.close();
}

int main()
{
	clock_t start = clock();
	loadPatternSet();
	loadWeightMtrxSet();
	loadVaildationPatternSet();
	//	runNoiseRecallTest();
	runTest();
	clock_t end = clock();
	std::cout << "duration = " << double(end - start) / CLOCKS_PER_SEC << "sec.\n";
	return 0;
}
