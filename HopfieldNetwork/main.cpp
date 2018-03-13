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

TestData test_data;
array<VectorXd, 10> pattern_set;
array<VectorXd, 10> validation_set;
MatrixXd weight_mtrx(PIXEL,PIXEL);

vector<string> split(const string& str, char sep)
{
	vector<string> v;
	stringstream ss(str);
	string buffer;
	while (getline(ss, buffer, sep)) { v.push_back(buffer); }
	return v;
}

void make_pattern_set()
{
	TrainData trainData;
	trainData.load();
	cout << "data loaded successfully!" << endl;
	trainData.calc_average_numeric();
	trainData.save_patterns();
	pattern_set = trainData.patterns;
}

void load_vaildation_pattern_set()
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
		VectorXd tmp_pattern = VectorXd::Zero(p.size());
		for (int i = 0; i < p.size(); ++i) { tmp_pattern[i] = stod(p[i]); }
		validation_set[index] = tmp_pattern;
	}
	cout << "Done." << endl;
	ifs.close();
}

void render_num(VectorXd data, ostream& out = cout)
{
	int n = int(sqrt(data.size()));
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			double a = double(data[i * n + j]);
			out << a;
			if (j < n - 1) { out << ","; }
		}
		out << endl;
	}
}

void export_matrix(MatrixXd mtrx, ostream& out = cout)
{
	for (int i = 0; i < mtrx.rows(); ++i)
	{
		for (int j = 0; j < mtrx.cols(); ++j)
		{
			out << mtrx(i, j);
			if (j < mtrx.rows() - 1) { out << ","; }
		}
		out << endl;
	}
}

MatrixXd calc_q_matrix()
{
	cout << "calculating q matrix..." << endl;
	int lim = pattern_set.size();
	MatrixXd result = MatrixXd::Zero(lim, lim);
	for (int i = 0; i < lim; ++i)
	{
		for (int j = 0; j < lim; ++j) { result(i, j) = double(pattern_set[i].dot(pattern_set[j])) / double(PIXEL); }
	}
	cout << endl << "Done." << endl;
	return result.inverse();
}

void calc_weight_matrix()
{
	cout << "calculating weight matrix..." << endl;
	MatrixXd qMtrxInv = calc_q_matrix();
	int n = 0;
	int lim = PIXEL * PIXEL;
	Concurrency::parallel_for(0, PIXEL, 1, [&n, &lim, &qMtrxInv](int i)
	{
		for (int j = 0; j < PIXEL; ++j)
		{
			n++;
			double status = double(n * 100.0 / (lim - 1));
			cout << fixed << setprecision(1) << status << "%" << "\r" << flush;
			weight_mtrx(i, j) = 0.0;
			for (int k = 0; k < pattern_set.size(); ++k)
			{
				for (int l = 0; l < pattern_set.size(); ++l)
				{
					weight_mtrx(i, j) += double(pattern_set[k][i]) * qMtrxInv(k, l) * double(pattern_set[l][j]) / double(PIXEL);
				}
			}
		}
	});
	cout << endl << "Done." << endl;
	ofstream ofs(WEIGHT_MATRIX_FILENAME);
	export_matrix(weight_mtrx, ofs);
	ofs.close();
}

void load_weight_mtrx_set()
{
	ifstream ifs(WEIGHT_MATRIX_PATH);
	if (!ifs)
	{
		cout << "weight matirx file is not found" << endl;
		cout << "create weight matirx file..." << endl;
		calc_weight_matrix();
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
			if (rows >= weight_mtrx.rows()) { break; }
			double status = double(rows * 100.0 / (weight_mtrx.rows() - 1));
			if (progress.size() < int(status) / 5) { progress += "#"; }
			cout << "progress: " << setw(4) << right << fixed << setprecision(1) << (status) << "% " << progress << "\r" <<
				flush;
			vector<basic_string<char>> p = split(str, ',');
			for (int i = 0; i < p.size(); ++i) { weight_mtrx(rows, i) = stod(p[i]); }
			rows++;
		}
		cout << endl << "Done." << endl;
		ifs.close();
	}
}

void load_pattern_set()
{
	ifstream ifs(PATTERN_PATH);
	if (!ifs)
	{
		cout << "pattern file is not found" << endl;
		cout << "create patttern file..." << endl;
		make_pattern_set();
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
			VectorXd tmp_pattern = VectorXd::Zero(p.size());
			for (int i = 0; i < p.size(); ++i) { tmp_pattern[i] = stod(p[i]); }
			pattern_set[index] = tmp_pattern;
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

tuple<int, double> verify_pattern(const VectorXd& input, bool output = false)
{
	array<double, 10> error = {};
	double min;
	int num;
	//	array<VectorXd, 10> answerSet = validationSet;
	array<VectorXd, 10> answer_set = pattern_set;
	for (int i = 0; i < answer_set.size(); ++i)
	{
		error[i] = 0.0;
		for (int j = 0; j < input.size(); ++j) { error[i] += (answer_set[i][j] - input[j]) * (answer_set[i][j] - input[j]); }
		error[i] /= double(input.size());
		if (i == 0 || error[i] < min)
		{
			min = error[i];
			num = i;
		}
		if (output) { cout << i << ", error " << scientific << setprecision(2) << error[i] << endl; }
	}
	return forward_as_tuple(num, error[num]);
}

double activation_func(double input)
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
auto act_func = [](const double input) { return tanh(input / u0); };

VectorXd activation_func(const VectorXd& inputs) { return inputs.unaryExpr(act_func); }

auto inverse_act_func = [](const double input) { return 0.5 * u0 * log((1.0 + input) / (1.0 - input)); };

VectorXd inverse_activation_func(const VectorXd& inputs) { return inputs.unaryExpr(inverse_act_func); }

VectorXd update_vector(const VectorXd& vctr, int index)
{
	VectorXd result = vctr;
	//calculate input
	double input_val = weight_mtrx.col(index).dot(vctr);
	//ignite validation
	double next_val = activation_func(input_val);
	//update index value
	result[index] = next_val;
	return result;
}

VectorXd calc_delta_u(const VectorXd& state, const VectorXd& innerVal)
{
	const double tau = 1.0;
	const double delta_t = 0.01;
	VectorXd delta = (-innerVal / tau + weight_mtrx * state) * delta_t;
	return delta;
}

void noise_recall(const VectorXd& input, ostream& out = cout)
{
	VectorXd result = input;
	VectorXd inner_val = inverse_activation_func(input);
	for (int i = 0; i < RECALL_TIME; ++i)
	{
		cout << "progress: " << i << " / " << RECALL_TIME << " \r" << flush;
		if (i == 0)
		{
			out << "before:" << endl;

			render_num(result, out);
			out << endl << endl;
		}
		//calc delta innerVal

		//update innerVal
		inner_val += calc_delta_u(result, inner_val);
		//update state from innerVal
		result = activation_func(inner_val);
	}
	out << "after " << RECALL_TIME << " iterations" << endl;

	double error;
	int num;
	tie(num, error) = verify_pattern(result, true);
	cout << "recalled: " << num << ", error " << scientific << setprecision(3) << error << endl;
	render_num(result, out);
	out << endl << endl;
}

int recall_test(const VectorXd& input, int ans, ostream& out = cout)
{
	VectorXd result = input;
	VectorXd inner_val = inverse_activation_func(input);
	for (int i = 0; i < RECALL_TIME; ++i)
	{
		//calc delta innerVal

		//update innerVal
		inner_val += calc_delta_u(result, inner_val);
		//update state from innerVal
		result = activation_func(inner_val);
	}
	int num;
	double error;
	tie(num, error) = verify_pattern(result);
	out << "answer: ," << ans << ",recalled: ," << num << ", error," << scientific << setprecision(3) << error;

	tie(num, error) = verify_pattern(input);
	out << ",default validate: ," << num << endl;
	return num;
}

void run_noise_recall_test()
{
	//	testData.load();
	ofstream ofs("noise.csv");
	for (int i = 0; i < 10; ++i)
	{
		//		VectorXd testMtrx = testData.images[i];
		VectorXd test_mtrx = pattern_set[i];
		//add noise
		for (int j = 0; j < 200; ++j)
		{
			int n = rand() % test_mtrx.size();
			test_mtrx[n] *= -1;
		}
		noise_recall(test_mtrx, ofs);
	}
	ofs.close();
}

void run_test()
{
	test_data.load();
	ofstream ofs("testResult.csv");
	array<int, 10> trial = {};
	array<int, 10> correct = {};
	int n = test_data.labels.size();
	int sum = 1;
	//	float progress;
	cout << endl;

	Concurrency::parallel_for(0, n, 1, [&sum, &ofs, &trial, &correct](int i)
	{
		cout << "progress: " << sum << " / " << test_data.labels.size() << " \r" << flush;
		int result = recall_test(test_data.images[i], test_data.labels[i], ofs);
		trial[test_data.labels[i]] += 1;
		if (result == test_data.labels[i]) { correct[test_data.labels[i]] += 1; }
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
	load_pattern_set();
	load_weight_mtrx_set();
	load_vaildation_pattern_set();
	//	runNoiseRecallTest();
	run_test();
	clock_t end = clock();
	cout << "duration = " << double(end - start) / CLOCKS_PER_SEC << "sec.\n";
	return 0;
}
