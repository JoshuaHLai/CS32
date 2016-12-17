#ifndef HISTORY_DEFINED
#define HISTORY_DEFINED

#include "globals.h"

using namespace std;

class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int m_Historyrows;
	int m_Historycols;
	char historyGrid[MAXROWS][MAXCOLS];
	int newLandingPoint[MAXROWS][MAXCOLS];

};

#endif		//HISTORY_DEFINED