#include "History.h"
#include "globals.h"

#include <iostream>

History::History(int nRows, int nCols)
{
	m_Historyrows = nRows;
	m_Historycols = nCols;

	//For the History Grid
	for (int r = 0; r < nRows; r++)
		for (int c = 0; c < nCols; c++)
			historyGrid[r][c] = '.';

	////For the new point where the player lands
	for (int r = 0; r < m_Historyrows; r++)
		for (int c = 0; c < m_Historycols; c++)
			newLandingPoint[r][c] = 0;
}

bool History::record(int r, int c)
{
	if (r < 1 || r > m_Historyrows || c < 1 || c > m_Historycols)
		return false;
	else
	{
		newLandingPoint[r - 1][c - 1]++;
		if (newLandingPoint[r - 1][c - 1] == 1)
			historyGrid[r - 1][c - 1] = 'A';
		else if (newLandingPoint[r - 1][c - 1] > 26)
			historyGrid[r - 1][c - 1] = 'Z';
		else
			historyGrid[r - 1][c - 1]++;
		return true;
	}
}

void History::display() const
{
	clearScreen();
	for (int r = 0; r < m_Historyrows; r++)
	{
		for (int c = 0; c < m_Historycols; c++)
			cout << historyGrid[r][c];
		cout << endl;
	}
	cout << endl;
}