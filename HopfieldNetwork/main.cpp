// HopfieldNetwork.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "DataSet.h"


int main()
{
	DataSet mnist;
	mnist.readTrainingFile("./MNISTData/train-images-idx3-ubyte/train-images.idx3-ubyte");
	mnist.readLabelFile("./MNISTData/train-labels-idx1-ubyte/train-labels.idx1-ubyte");
    return 0;
}