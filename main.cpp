#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

using namespace std;

int comparator(pair<string, int> a, pair<string, int> b) {
	if (a.second == b.second) {
		return a.first < b.first;
	}
	return a.second > b.second;
}

int main(int argc, char* argv[]) {

	ifstream ifile(argv[1]);
	string ins;
	int total{ 0 };
	
	map<string, int> Data;

	while (getline(ifile, ins, '\n')) {
		string curs;
		for (int i = 0; i < ins.length(); i++) {
			if (isalpha(ins[i]) || isdigit(ins[i])) {
				curs += ins[i];
			} else if (!curs.empty()) {
				Data[curs] += 1;
				total++;
				curs = "";
			}
		}

		if (!curs.empty()) {
			Data[curs] += 1;
			total++;
			curs = "";
		}
	}

	vector<pair<string, int>> SortedVector(Data.begin(), Data.end());

	stable_sort(SortedVector.begin(), SortedVector.end(), comparator);

	ofstream ofile(argv[2]);
	for (auto i : SortedVector) {
		ofile << i.first << ',' << i.second << ',' << ( 100.0f * i.second / total ) << '%' << endl;
	}

	return 0;
}
