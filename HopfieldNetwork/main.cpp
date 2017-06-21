// HopfieldNetwork.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "DataSet.h"
#include "parameters.h"
#include "Eigen/Core"

DataSet train;
DataSet test;

void readData()
{
	train.readTrainingFile(TRAIN_IMAGE_PATH);
	train.readLabelFile(TRAIN_LABEL_PATH);
	//	test.readTrainingFile(TEST_IMAGE_PATH);
	//	test.readLabelFile(TEST_LABEL_PATH);
	return;
}

ofstream ofs("asdfasdf.csv"); //ファイル出力ストリーム

void renderNumber(VectorXd data)
{
	int n = int(sqrt(PIXEL));
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			int a = int(data[i * n + j] / 255 + 0.7);
			cout << a << " ";
			ofs << a << " ";
		}
		cout << endl;
		ofs << endl;
	}
	ofs << endl;
}

void calcAverageNumeric()
{
	VectorXd patterns[10] = {VectorXd::Zero(PIXEL), VectorXd::Zero(PIXEL), VectorXd::Zero(PIXEL), VectorXd::Zero(PIXEL), VectorXd::Zero(PIXEL), VectorXd::Zero(PIXEL), VectorXd::Zero(PIXEL), VectorXd::Zero(PIXEL), VectorXd::Zero(PIXEL), VectorXd::Zero(PIXEL)};
	int counts[10] = {0};
	for (int i = 0; i < train.data.size(); ++i)
	{
		patterns[int(train.label[i])] += train.data[i];
		counts[int(train.label[i])] += 1;
	}

	for (int j = 0; j < 10; ++j)
	{
		if (counts[j] != 0)
		{
			patterns[j] /= counts[j];
		}
		renderNumber(patterns[j]);
		cout << endl;
	}
}

int main()
{
	readData();
	cout << "data loaded!" << endl;
	//	for (int i = 0; i < train.trainingData.size(); ++i)
	//	{
	//	cout << test.label[0] << endl;
	//	renderNumber(test.data[0]);
	//	}
	calcAverageNumeric();
	return 0;
}
