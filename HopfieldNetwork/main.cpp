// HopfieldNetwork.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "TrainData.h"
#include "TestData.h"
#include <fstream>

#include <vector>
#include <string>
#include <sstream>

//TestData testData;
array<VectorXd, 10> patternSet;

std::vector<std::string> split(const std::string& str, char sep)
{
	std::vector<std::string> v;
	std::stringstream ss(str);
	std::string buffer;
	while (std::getline(ss, buffer, sep))
	{
		v.push_back(buffer);
	}
	return v;
}

void makePatternSet()
{
	TrainData trainData;
	trainData.load();
	cout << "data loaded!" << endl;
	trainData.calcAverageNumeric();
	trainData.savePatterns();
	patternSet = trainData.patterns;
}

void renderNum(VectorXd data, ostream& out = cout)
{
	int n = int(sqrt(data.size()));
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			int a = int(data[i * n + j]);
			out << a;
			if (j < n - 1)
			{
				out << ",";
			}
		}
		out << endl;
	}
}

void loadPatternSet()
{
	ifstream ifs(PATTERN_PATH);
	if (!ifs)
	{
		cout << "pattern file not found" << endl;
		cout << "creating patttern file..." << endl;
		makePatternSet();
	}
	else
	{
		//csvファイルを1行ずつ読み込む
		string str;
		while (getline(ifs, str))
		{
			vector<basic_string<char>> p = split(str, ',');
			int index = stoi(p[0]);
			p.erase(p.begin());
			cout << index << "; " << p.size() << endl;
			VectorXd tmpPattern = VectorXd::Zero(p.size());
			for (int i = 0; i < p.size(); ++i)
			{
				tmpPattern[i] = stoi(p[i]);
			}
			patternSet[index] = tmpPattern;
		}
		cout << "Done." << endl;
	}
	for (int i = 0; i < 10; ++i)
	{
		renderNum(patternSet[i]);
		cout << endl;
	}
}

int main()
{
	loadPatternSet();
	return 0;
}
