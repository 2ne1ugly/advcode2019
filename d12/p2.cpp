	#include <iostream>
	#include <string>
	#include <tuple>
	#include <math.h>
	#include <boost/algorithm/clamp.hpp>
	#include <boost/functional/hash.hpp>
	#include <boost/rational.hpp>
	#include <unordered_map>
	#include <array>
	#include <algorithm>

	using namespace std;
	using namespace boost::algorithm;

	int main() {
		array<array<int, 3>, 4> positions = {{
			{9, 13, -8},
			{-3, 16, -17},
			{-4, 11, -10},
			{0, -2, -2}
		}};

		array<array<int, 3>, 4> velocities = {{
			{0, 0, 0},
			{0, 0, 0},
			{0, 0, 0},
			{0, 0, 0}
		}};

		unordered_map<array<pair<int, int>, 4>, int, boost::hash<array<pair<int, int>, 4>>> prev[3];
		array<int, 3> start;
		array<int, 3> loop;
		start.fill(-1);
		loop.fill(-1);

		size_t t = 0;
		while (any_of(start.begin(), start.end(), [](int val){return val == -1;})) {
			for (int i = 0; i < 3; i++) {
				if (start[i] == -1) {
					prev[i][{{
						{positions[0][i], velocities[0][i]},
						{positions[1][i], velocities[1][i]},
						{positions[2][i], velocities[2][i]},
						{positions[3][i], velocities[3][i]}
					}}] = t;
					for (int j = 0; j < 4; j++) {
						for (int k = j + 1; k < 4; k++) {
							int delta = positions[k][i] - positions[j][i];
							delta = clamp(delta, -1, 1);
							velocities[j][i] += delta; 
							velocities[k][i] -= delta; 
						}
					}
					for (int j = 0; j < 4; j++) {
						positions[j][i] += velocities[j][i];
					}
					if (prev[i].count({{
							{positions[0][i], velocities[0][i]},
							{positions[1][i], velocities[1][i]},
							{positions[2][i], velocities[2][i]},
							{positions[3][i], velocities[3][i]}
							}}) == 1) {
						start[i] = prev[i][{{
							{positions[0][i], velocities[0][i]},
							{positions[1][i], velocities[1][i]},
							{positions[2][i], velocities[2][i]},
							{positions[3][i], velocities[3][i]}
							}}];
						loop[i] = (t + 1) - start[i];
					}
				}
			}
			t++;
			if (t % 1000000 == 0)
				cout << t << endl;
		}
		cout << start[0] << " " << loop[0] << endl;
		cout << start[1] << " " << loop[1] << endl;
		cout << start[2] << " " << loop[2] << endl;
		int firstStart = max(max(start[0], start[1]), start[2]);
		int lcm = boost::lcm(boost::lcm(loop[0], loop[1]), loop[2]);
		int firstRepeat = lcm;
		while (firstRepeat <= firstStart) {
			firstRepeat += lcm;
		}
		cout << firstRepeat << endl;
	}