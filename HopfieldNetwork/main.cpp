// HopfieldNetwork.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "TrainData.h"
#include "TestData.h"

TrainData trainData;
TestData testData;

int main()
{
	trainData.load();
	cout << "data loaded!" << endl;
	trainData.setWrite();
	trainData.calcAverageNumeric();
	return 0;
}
