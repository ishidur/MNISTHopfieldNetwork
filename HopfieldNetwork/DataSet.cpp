#include "stdafx.h"
#include "DataSet.h"

//バイト列からintへの変換
int reverseInt(int i)
{
	unsigned char c1 = i & 255;
	unsigned char c2 = (i >> 8) & 255;
	unsigned char c3 = (i >> 16) & 255;
	unsigned char c4 = (i >> 24) & 255;

	return (int(c1) << 24) + (int(c2) << 16) + (int(c3) << 8) + c4;
}

vector<VectorXd> DataSet::readImageFile(string filename)
{
	ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
	int magic_number = 0;
	int number_of_images = 0;
	int rows = 0;
	int cols = 0;

	//ヘッダー部より情報を読取る。
	ifs.read((char*)&magic_number, sizeof(magic_number));
	magic_number = reverseInt(magic_number);
	ifs.read((char*)&number_of_images, sizeof(number_of_images));
	number_of_images = reverseInt(number_of_images);
	ifs.read((char*)&rows, sizeof(rows));
	rows = reverseInt(rows);
	ifs.read((char*)&cols, sizeof(cols));
	cols = reverseInt(cols);

	vector<VectorXd> images(number_of_images);
	cout << magic_number << " " << number_of_images << " " << rows << " " << cols << endl;

	for (int i = 0; i < number_of_images; i++)
	{
		images[i] = VectorXd::Zero(PIXEL);

		for (int row = 0; row < rows; row++)
		{
			for (int col = 0; col < cols; col++)
			{
				unsigned char temp = 0;
				ifs.read((char*)&temp, sizeof(temp));
				images[i][rows * row + col] = (double)temp;
			}
		}
	}
	return images;
}

vector<double> DataSet::readLabelFile(string filename)
{
	ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
	int magic_number = 0;
	int number_of_images = 0;
	//ヘッダー部より情報を読取る。
	ifs.read((char*)&magic_number, sizeof(magic_number));
	magic_number = reverseInt(magic_number);
	ifs.read((char*)&number_of_images, sizeof(number_of_images));
	number_of_images = reverseInt(number_of_images);

	vector<double> _label(number_of_images);

	cout << number_of_images << endl;

	for (int i = 0; i < number_of_images; i++)
	{
		unsigned char temp = 0;
		ifs.read((char*)&temp, sizeof(temp));
		_label[i] = double(temp);
	}
	return _label;
}

void DataSet::renderNumber(VectorXd data, ostream& out)
{
	int n = int(sqrt(data.size()));
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			int a = int(data[i * n + j] / 255 + 0.7);
			out << a;
			if (j < n - 1)
			{
				out << ",";
			}
		}
		out << endl;
	}
}
