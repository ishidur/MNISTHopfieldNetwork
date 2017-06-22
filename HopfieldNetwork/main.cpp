// HopfieldNetwork.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "TrainData.h"
#include "TestData.h"
#include <array>

TrainData trainData;
TestData testData;

//ofstream ofs("asdfasdf.csv"); //ファイル出力ストリーム

void renderNumber(VectorXd data)
{
	int n = int(sqrt(data.size()));
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			int a = int(data[i * n + j] / 255 + 0.7);
			cout << a << " ";
			//			ofs << a << " ";
		}
		cout << endl;
		//		ofs << endl;
	}
	//	ofs << endl;
}

void calcAverageNumeric()
{
	array<VectorXd,10> patterns;
	patterns.fill(VectorXd::Zero(PIXEL));
	int counts[10] = {0};
	for (int i = 0; i < trainData.images.size(); ++i)
	{
		patterns[int(trainData.labels[i])] += trainData.images[i];
		counts[int(trainData.labels[i])] += 1;
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
	trainData.load();
	cout << "data loaded!" << endl;
	//		for (int i = 0; i < trainData.images.size(); ++i)
	//		{
	//		cout << testData.labels[0] << endl;
	//		renderNumber(testData.images[0]);
	//		}
	calcAverageNumeric();
	return 0;
}
