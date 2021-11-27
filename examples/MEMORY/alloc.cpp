#include <iostream>
#include <vector>
#include <stack>

using namespace std;

vector<int> mem_req; 
vector<int> h, hole;
vector<vector<int>> result;

void ff(vector<int> ohole) {
	for (int i = 0; i < mem_req.size(); ++i) {
		bool bit = false;
		for (int j = 0; j < h.size(); ++j) {
			if (ohole[j] > mem_req[i]) {
				result[0][i] = h[j];
				ohole[j] -= mem_req[i];
				bit = true;
				goto END;
			}
		}
		END: if (!bit) result[0][i] = -1;
	}
}

struct item { int hole; int pos; };

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
			if (ohole[j] > mem_req[i]) save.push({h[j], j});
		}

		if (save.empty()) result[1][i] = -1;
		else {
			item min = findmin(save);
			result[1][i] = h[min.pos];
			ohole[min.pos] -= mem_req[i];
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
			if (ohole[j] > mem_req[i]) save.push({h[j], j});
		}

		if (save.empty()) result[2][i] = -1;
		else {
			item max = findmax(save);
			result[2][i] = h[max.pos];
			ohole[max.pos] -= mem_req[i];
		}
	}
}

int main() {
	size_t x, y, xsize, ysize, i = 0, k = 0;
	cout << "Enter Memory Requirements Size: "; cin >> xsize;
	cout << "Enter Memory Requirements: ";
	while (i++ < xsize) { cin >> x; mem_req.push_back(x); }
	cout << "Enter Memory Hole Size: "; cin >> ysize;
	cout << "Enter Memory Holes: ";
	while (k++ < ysize) { cin >> y; h.push_back(y); hole.push_back(y); }
	cout << "\n\n";

	for (int i = 0; i < 3; ++i) {
		result.push_back({});
		for (int j = 0; j < xsize; ++j) result[i].push_back(0);
	}

	ff(hole); bf(hole); wf(hole);

	for (vector<int> l : result) {
		for (int s : l) {
			if (s == -1) cout << "wait ";
			else cout << s << "kb ";
		}
		cout << endl;
	}
	return 0;
}
