#ifndef HISTORY_H
#define HISTORY_H
#include "globals.h"

class History {
private:
	int m_rows;
	int m_cols;
	char m_hGrid[MAXROWS][MAXCOLS];
	
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
};








#endif