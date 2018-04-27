#include <iostream>

using namespace std;

// n is the total duration of the invasion in seconds
const int n = 4;

// Firing time array: 1 for FIRE, 0 for HOLD
bool fire[4];

// Drone arrival quantities
int x_i[4] = { 1, 10, 10, 1 };

// Lazer charge function
int f_i[4] = { 1, 2, 4, 8 };

// Matrix filled with all possible firing/holding combinations; dimensions are n+1 x n+1; initialized to 0
int combos[n + 1][n + 1] = { {0, 0, 0, 0, 0} };
int disabled[n + 1][n + 1] = { { 0, 0, 0, 0, 0 } };

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


// Update part b to enable the reconstruction of the optimal solution for the best times to fire the laser (auxilliary table)
int main() {
	// Load combos matrix with all possible firing times
	for (int i = 1; i <= n; i++) {						// rows
		for (int j = 1; j <= i; j++) {					// cols
			// Add the current index with the 
			combos[i][j] = min(x_i[i - 1], f_i[j-1]);
		}
	}


	// Max value for each row
	int max = 0;
	int maxColIndex[4] = { 0 };

	// Load alternate array for optimal solution for disabled 
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= i; j++) {
			disabled[i][j] = combos[i][j] + disabled[i - j][maxColIndex[i - j]];
			if (disabled[i][j] > max) {
				// Update max value
				max = disabled[i][j];
				// Update max index
				maxColIndex[i] = j;
			}
		}
		// Reset the maximum for next row
		max = 0;
	}

	// Print auxilliary table
	cout << endl;
	cout << '\t' << "Auxilliary Decision Matrix" << endl;
	cout << '\t' << "==========================" << endl;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << '\t' << disabled[i][j];
		}
		cout << endl;
	}
	cout << endl;

	return 0;
}