#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

template <typename T>
void print(vector<T> const &v, int k) {
	cout << k << (k < 10 ? " | " : "| ");
    for (auto i : v) cout << i << ' ';
    cout << endl;
}

template <typename T>
vector<T> slice(vector<T> const &v, int m, int n) {
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;
 
    vector<T> vec(first, last);

    return vec;
}

template <typename T>
void removeDup(vector<T>& ws) {
	sort(ws.begin(), ws.end());
	ws.erase(unique(ws.begin(), ws.end()), ws.end());
}

template <typename T>
void work(const vector<T>& a, const size_t& delta) {
	vector<T> ws;

	int j = 0;
	while (j < delta-1) {
		ws.push_back(a[j++]);
		removeDup(ws);
		print(ws, j);
	}

	for (size_t i = 0; i <= a.size()-delta; ++i) {
		ws = slice(a, i, i+delta-1);
		removeDup(ws);
		print(ws, i+delta);
	}
}

int main() {
	size_t size, delta, iinput, type;
	char cinput;
	cout << "Enter Data Type (1=char | 2=int): "; cin >> type;
	if (type == 1) {
		vector<char> a;

		cout << "Enter a Size: "; cin >> size;
		cout << "Enter Delta Value: "; cin >> delta;
		cout << "Enter a Reference String: ";
		for (int i = 0; i < size; ++i) { cin >> cinput; a.push_back(cinput); }
		cout << endl;
		work(a, delta);
	}
	
	if (type == 2) {
		vector<int> a;

		cout << "Enter a Size: "; cin >> size;
		cout << "Enter Delta Value: "; cin >> delta;
		cout << "Enter a Reference String: ";
		for (int i = 0; i < size; ++i) { cin >> iinput; a.push_back(iinput); }
		cout << endl;
	}
	return 0;
}
