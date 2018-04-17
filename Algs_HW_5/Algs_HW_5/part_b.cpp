#include <iostream>

using namespace std;

// Firing time array: 1 for FIRE, 0 for HOLD
bool fire[4];

// Total time of invasion
int n = 3;

// Drone arrival quantities
int x_i[4] = { 1, 10, 10, 1 };

// Lazer charge function
int f_i[4] = { 1, 2, 4, 8 };

// Max function: if NOW is the max value, FIRE, if NEXT is the max value HOLD
bool max(int now, int next) {
	if (now > next) {
		return 1;
	}

	else {
		return 0;
	}
}

// Min function: return the minimum value
int min(int drones, int charge) {
	if (drones < charge) {
		return drones;
	}

	else {
		return charge;
	}
}

// Recursive solution
int opt(int x_i_index, int f_i_index) {
	if (x_i_index >= n) {
		fire[x_i_index] = true;
		return 0;
	}

	else if (x_i_index < n) {
		fire[x_i_index] = max(min(x_i[x_i_index], f_i[f_i_index]) + opt(x_i_index + 1, 0), opt(x_i_index + 1, f_i_index + 1));
		return min(x_i[x_i_index], f_i[f_i_index]);
	}
}

// Write an algorithm which computes the optimal solution the the problem based on the recurrence developed in part a
int main() {

	// Recursive solution starting from first second, ending at n seconds
	int max = opt(0, 0);

	// Print table for which seconds to fire at
	cout << endl;
	cout << '\t' << "Firing Schedule" << endl;
	cout << '\t' << "===============" << endl;
	cout << "Sec:";
	for (int i = 1; i <= n + 1; i++) {
		cout << '\t' << i;
	}
	cout << endl;
	cout << "Fire:";
	for (int i = 0; i <= n; i++) {
		cout << '\t';
		if (fire[i]) {
			cout << 'X';
		}
		else {
			cout << ' ';
		}
	}
	cout << endl << endl;

	return 0;
}