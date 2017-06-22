// HopfieldNetwork.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "TrainData.h"
#include "TestData.h"

TestData testData;

void makePatternSet()
{
	TrainData trainData;
	trainData.load();
	cout << "data loaded!" << endl;
	trainData.setWrite();
	trainData.calcAverageNumeric();
}

int main()
{
	makePatternSet();
	return 0;
}
