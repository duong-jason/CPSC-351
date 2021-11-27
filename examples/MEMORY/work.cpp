#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

template<typename T>
void print(vector<T> const &v, int k) {
	cout << k << (k < 10 ? " | " : "| ");
	for (auto i : v) cout << i << ' ';
	cout << endl;
}

template<typename T>
vector<T> slice(vector<T> const &v, int m, int n) {
	auto first = v.cbegin() + m;
	auto last = v.cbegin() + n + 1;
 
	vector<T> vec(first, last);

	return vec;
}

void removeDup(vector<int>& ws) {
	sort(ws.begin(), ws.end());
	ws.erase(unique(ws.begin(), ws.end()), ws.end());
}

void work(const vector<int>& a, const size_t& delta) {
	vector<int> ws;

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
	size_t size, delta, input;
	vector<int> a;

	cout << "Enter a Size: "; cin >> size;
	cout << "Enter Delta Value: "; cin >> delta;
	cout << "Enter a Reference String: ";
	for (int i = 0; i < size; ++i) { cin >> input; a.push_back(input); }
	cout << endl;
	work(a, delta);
	return 0;
}
