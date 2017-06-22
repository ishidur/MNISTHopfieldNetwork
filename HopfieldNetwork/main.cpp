// HopfieldNetwork.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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
	trainData.calcAverageNumeric();
	trainData.savePatterns();
}

int main()
{
	makePatternSet();
	return 0;
}
