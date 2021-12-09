#include <iostream>
#include <vector>
#include <iomanip>
#include <stack>

using namespace std;

vector<int> mem_req; 
vector<int> h, hole;
vector<vector<int>> result;
vector<vector<string>> calc;

struct item { int hole; int pos; };

void ff(vector<int> ohole) {
	for (int i = 0; i < mem_req.size(); ++i) {
		bool bit = false;
		for (int j = 0; j < h.size(); ++j) {
			if (ohole[j] >= mem_req[i]) {
				result[0][i] = h[j];
				calc[0][i] = to_string(ohole[j]) + "-";
				calc[0][i] += to_string(mem_req[i]);
				calc[0][i] += "=";

				ohole[j] -= mem_req[i];

				calc[0][i] += to_string(ohole[j]);

				bit = true;
				break;
			}
		}
		if (!bit) result[0][i] = -1;
	}
}

item findmin(stack<item>& save) {
	item min = save.top();
	save.pop();

	while (!save.empty()) {
		if (save.top().hole <= min.hole) min = {save.top().hole, save.top().pos};
		save.pop();
	}

	return min;
}

void bf(vector<int> ohole) {
	stack<item> save;
	for (int i = 0; i < mem_req.size(); ++i) {
		for (int j = 0; j < h.size(); ++j) {
			if (ohole[j] >= mem_req[i]) save.push({ohole[j], j});
		}

		if (save.empty()) result[1][i] = -1;
		else {
			item min = findmin(save);
			result[1][i] = h[min.pos];

			calc[1][i] = to_string(ohole[min.pos]) + "-";
			calc[1][i] += to_string(mem_req[i]);
			calc[1][i] += "=";

			ohole[min.pos] -= mem_req[i];

			calc[1][i] += to_string(ohole[min.pos]);
		}
	}
}

item findmax(stack<item>& save) {
	item max = save.top();
	save.pop();

	while (!save.empty()) {
		if (save.top().hole >= max.hole) max = {save.top().hole, save.top().pos};
		save.pop();
	}

	return max;
}

void wf(vector<int> ohole) {
	stack<item> save;
	for (int i = 0; i < mem_req.size(); ++i) {
		for (int j = 0; j < h.size(); ++j) {
			if (ohole[j] >= mem_req[i]) save.push({ohole[j], j});
		}

		if (save.empty()) result[2][i] = -1;
		else {
			item max = findmax(save);
			result[2][i] = h[max.pos];

			calc[2][i] = to_string(ohole[max.pos]) + "-";
			calc[2][i] += to_string(mem_req[i]);
			calc[2][i] += "=";

			ohole[max.pos] -= mem_req[i];

			calc[2][i] += to_string(ohole[max.pos]);
		}
	}
}

int main() {
	size_t x, y, xsize, ysize;
	cout << "Enter Memory Requirements Size: "; cin >> xsize;
	cout << "Enter Memory Requirements: ";
	while (mem_req.size() < xsize && cin >> x) mem_req.push_back(x);
	cout << "Enter Memory Hole Size: "; cin >> ysize;
	cout << "Enter Memory Holes: ";
	while (hole.size() < ysize && cin >> y) { h.push_back(y); hole.push_back(y); }

	for (int i = 0; i < xsize-1; ++i) {
		calc.push_back({""});
		for (int j = 0; j < 3; ++j) {
			calc[i].push_back("");	
		}
	}

	cout << "\n\n";
	for (int i = 0; i < 3; ++i) {
		result.push_back({});
		for (int j = 0; j < xsize; ++j) result[i].push_back(0);
	}

	ff(hole); bf(hole); wf(hole);

	int z = 1;
	cout << "Memory Holes: "; for (size_t h : hole) { cout << h << " ";  } cout << endl;
	string t; cout << "Process "; for (size_t v : mem_req) { cout << v << " "; t += to_string(v); } cout << endl;
	for (int i = 0; i < t.length()+11; ++i) { cout << "-"; } cout << endl;

	for (vector<int> l : result) {
		switch(z) {
			case 1: cout << "First-Fit | "; break;
			case 2: cout << "Best-Fit  | "; break;
			case 3: cout << "Worst-Fit | "; break;
			default: break;
		}
		for (int s : l) {
			if (s == -1) cout << "wait ";
			else cout << s << "kb ";
		}
		z++;
		cout << endl;
	}

	cout << endl << endl;
	cout << "Calculations: \n--------------------------\n";
	for (vector<string> c : calc) {
		for (string s : c) {
			cout << s << setw(12-s.length()) << "|";
		}
		cout << endl;
	}
	cout << endl;

	return 0;
}
