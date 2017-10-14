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
	cout << "data loaded successfully!" << endl;
	trainData.calcAverageNumeric();
	trainData.savePatterns();
	patternSet = trainData.patterns;
}

void loadVaildationPatternSet()
{
	ifstream ifs(VALIDATION_DATA_PATH);
	cout << "loading from validation file" << endl;
	//csvファイルを1行ずつ読み込む
	string str;
	while (getline(ifs, str))
	{
		vector<basic_string<char>> p = split(str, ',');
		int index = stod(p[0]);
		p.erase(p.begin());
		cout << index << "; " << p.size() << " pixels" << endl;
		VectorXd tmpPattern = VectorXd::Zero(p.size());
		for (int i = 0; i < p.size(); ++i)
		{
			tmpPattern[i] = stod(p[i]);
		}
		validationSet[index] = tmpPattern;
	}
	cout << "Done." << endl;
	ifs.close();
}

void renderNum(VectorXd data, ostream& out = cout)
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
			result(i, j) = double(patternSet[i].dot(patternSet[j])) / double(PIXEL);
		}
	}
	cout << endl << "Done." << endl;
	return result.inverse();
}

void calcWeightMatrix()
{
	cout << "calculating weight matrix..." << endl;
	MatrixXd qMtrxInv = calcQMatrix();
	int n = 0;
	int lim = PIXEL * PIXEL;
	Concurrency::parallel_for(0, PIXEL, 1, [&n, &lim, &qMtrxInv](int i)
	{
		for (int j = 0; j < PIXEL; ++j)
		{
			n++;
			double status = double(n * 100.0 / (lim - 1));
			cout << fixed << setprecision(1) << status << "%" << "\r" << flush;
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
			cout << "progress: " << setw(4) << right << fixed << setprecision(1) << (status) << "% " << progress << "\r" <<
				flush;
			vector<basic_string<char>> p = split(str, ',');
			for (int i = 0; i < p.size(); ++i)
			{
				weightMtrx(rows, i) = stod(p[i]);
			}
			rows++;
		}
		cout << endl << "Done." << endl;
		ifs.close();
	}
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
			int index = stod(p[0]);
			p.erase(p.begin());
			cout << index << "; " << p.size() << " pixels" << endl;
			VectorXd tmpPattern = VectorXd::Zero(p.size());
			for (int i = 0; i < p.size(); ++i)
			{
				tmpPattern[i] = stod(p[i]);
			}
			patternSet[index] = tmpPattern;
		}
		cout << "Done." << endl;
		ifs.close();
	}
	//	for (int i = 0; i < 10; ++i)
	//	{
	//		renderNum(patternSet[i]);
	//		std::cout << endl;
	//	}
}

tuple<int, double> verifyPattern(const VectorXd& input, bool output = false)
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
			cout << i << ", error " << scientific << setprecision(2) << error[i] << endl;
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
	double result = 1.0 / (1.0 + exp(-input));
	return result;
}

const double u0 = 0.5;
auto actFunc = [](const double input) { return tanh(input / u0); };

VectorXd activationFunc(const VectorXd& inputs)
{
	return inputs.unaryExpr(actFunc);
}

auto inverseActFunc = [](const double input)
{
	return 0.5 * u0 * log((1.0 + input) / (1.0 - input));
};

VectorXd inverseActivationFunc(const VectorXd& inputs)
{
	return inputs.unaryExpr(inverseActFunc);
}

VectorXd updateVector(const VectorXd& vctr, int index)
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

VectorXd calcDeltaU(const VectorXd& state, const VectorXd& innerVal)
{
	const double tau = 1.0;
	const double deltaT = 0.01;
	VectorXd delta = (-innerVal / tau + weightMtrx * state) * deltaT;
	return delta;
}

void noiseRecall(const VectorXd& input, ostream& out = cout)
{
	VectorXd result = input;
	VectorXd innerVal = inverseActivationFunc(input);
	for (int i = 0; i < RECALL_TIME; ++i)
	{
		cout << "progress: " << i << " / " << RECALL_TIME << " \r" << flush;
		if (i == 0)
		{
			out << "before:" << endl;

			renderNum(result, out);
			out << endl << endl;
		}
		//calc delta innerVal

		//update innerVal
		innerVal += calcDeltaU(result, innerVal);
		//update state from innerVal
		result = activationFunc(innerVal);
	}
	out << "after " << RECALL_TIME << " iterations" << endl;

	double error;
	int num;
	tie(num, error) = verifyPattern(result, true);
	cout << "recalled: " << num << ", error " << scientific << setprecision(3) << error << endl;
	renderNum(result, out);
	out << endl << endl;
}

int recallTest(const VectorXd& input, int ans, ostream& out = cout)
{
	VectorXd result = input;
	VectorXd innerVal = inverseActivationFunc(input);
	for (int i = 0; i < RECALL_TIME; ++i)
	{
		//calc delta innerVal

		//update innerVal
		innerVal += calcDeltaU(result, innerVal);
		//update state from innerVal
		result = activationFunc(innerVal);
	}
	int num;
	double error;
	tie(num, error) = verifyPattern(result);
	out << "answer: ," << ans << ",recalled: ," << num << ", error," << scientific << setprecision(3) << error;

	tie(num, error) = verifyPattern(input);
	out << ",default validate: ," << num << endl;
	return num;
}

void runNoiseRecallTest()
{
	//	testData.load();
	ofstream ofs("noise.csv");
	for (int i = 0; i < 10; ++i)
	{
		//		VectorXd testMtrx = testData.images[i];
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
	cout << endl;

	Concurrency::parallel_for(0, n, 1, [&sum, &ofs, &trial, &correct](int i)
	{
		cout << "progress: " << sum << " / " << testData.labels.size() << " \r" << flush;
		int result = recallTest(testData.images[i], testData.labels[i], ofs);
		trial[testData.labels[i]] += 1;
		if (result == testData.labels[i])
		{
			correct[testData.labels[i]] += 1;
		}
		sum++;
	});

	cout << endl << "Done." << endl;
	ofs << endl;
	for (int i = 0; i < 10; ++i)
	{
		ofs << "accuracy," << i << ",%," << double(correct[i]) / double(trial[i]) * 100.0 << ",correct," << correct[i] <<
			",trial," << trial[i] << endl;
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
	cout << "duration = " << double(end - start) / CLOCKS_PER_SEC << "sec.\n";
	return 0;
}
