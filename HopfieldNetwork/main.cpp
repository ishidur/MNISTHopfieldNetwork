// HopfieldNetwork.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "DataSet.h"

DataSet mnist;

void readData()
{
	mnist.readTrainingFile("./MNISTData/train-images-idx3-ubyte/train-images.idx3-ubyte");
	mnist.readLabelFile("./MNISTData/train-labels-idx1-ubyte/train-labels.idx1-ubyte");
	return;
}

void renderNumber(vector<double> data)
{
	int n = int(sqrt(data.size()));
	cout << n << " X " << n << endl;
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
	//	for (int i = 0; i < mnist.trainingData.size(); ++i)
	//	{
	renderNumber(mnist.trainingData[0]);
	//	}
	return 0;
}
