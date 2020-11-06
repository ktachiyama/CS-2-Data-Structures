#include <iostream>
#include "History.h"
using namespace std;


History::History(int nRows, int nCols) {
	m_rows = nRows;
	m_cols = nCols;

	for (int r = 0; r < m_rows; r++)
		for (int c = 0; c < m_cols; c++)
			m_hGrid[r][c] = '.';
}

bool History::record(int r, int c) {
	if (r < 1 || r > m_rows || c < 1 || c > m_cols)
		return false;

	if (m_hGrid[r - 1][c - 1] == '.')
		m_hGrid[r - 1][c - 1] = 'A';

	else if (m_hGrid[r - 1][c - 1] == 'Z')
		m_hGrid[r - 1][c - 1] = 'Z';

	else 
		m_hGrid[r - 1][c - 1]++;

	return true;
}

void History::display() const {
	clearScreen();
	for (int r = 0; r != m_rows; r++) {
		for (int c = 0; c != m_cols; c++)
			cout << m_hGrid[r][c];
		cout << endl;
	}
	cout << "Press enter to continue";
}

