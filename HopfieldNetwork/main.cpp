// HopfieldNetwork.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "DataSet.h"
#include "parameters.h"

DataSet train;
DataSet test;

void readData()
{
	train.readTrainingFile(TRAIN_IMAGE_PATH);
	train.readLabelFile(TRAIN_LABEL_PATH);
	test.readTrainingFile(TEST_IMAGE_PATH);
	test.readLabelFile(TEST_LABEL_PATH);
	return;
}

void renderNumber(vector<double> data)
{
	int n = int(sqrt(data.size()));
	for (int i = 1; i <= n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout << int(data[i * j] / 255 + 0.5) << " ";
		}
		cout << endl;
	}
}

int main()
{
	readData();
	cout << "data loaded!" << endl;
	//	for (int i = 0; i < train.trainingData.size(); ++i)
	//	{
	renderNumber(train.trainingData[0]);
	//	}
	return 0;
}
