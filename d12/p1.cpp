#include <iostream>
#include <string>
#include <tuple>
#include <math.h>
#include <boost/algorithm/clamp.hpp>

using namespace std;
using namespace boost::algorithm;

int main() {

	tuple<int, int, int> positions[4] = {
		{9, 13, -8},
		{-3, 16, -17},
		{-4, 11, -10},
		{0, -2, -2}
	};

	tuple<int, int, int> velocities[4] = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

	
	for (int t = 0; t < 1000; t++) {
		//velocity update
		for (int i = 0; i < 4; i++) {
			for (int j = i + 1; j < 4; j++) {
				int dx = get<0>(positions[j]) - get<0>(positions[i]);
				int dy = get<1>(positions[j]) - get<1>(positions[i]);
				int dz = get<2>(positions[j]) - get<2>(positions[i]);
				dx = clamp(dx, -1, 1);
				dy = clamp(dy, -1, 1);
				dz = clamp(dz, -1, 1);
				get<0>(velocities[i]) += dx;
				get<1>(velocities[i]) += dy;
				get<2>(velocities[i]) += dz;
				get<0>(velocities[j]) -= dx;
				get<1>(velocities[j]) -= dy;
				get<2>(velocities[j]) -= dz;
			}
		}
		//position update
		for (int i = 0; i < 4; i++) {
			get<0>(positions[i]) += get<0>(velocities[i]);
			get<1>(positions[i]) += get<1>(velocities[i]);
			get<2>(positions[i]) += get<2>(velocities[i]);
		}
	}

	int sum = 0;
	for (int i = 0; i < 4; i++) {
		int potential = abs(get<0>(positions[i]))
			+ abs(get<1>(positions[i]))
			+ abs(get<2>(positions[i]));
		int kinetic = abs(get<0>(velocities[i]))
			+ abs(get<1>(velocities[i]))
			+ abs(get<2>(velocities[i]));
		sum += potential * kinetic;
	}
	cout << sum << endl;
}