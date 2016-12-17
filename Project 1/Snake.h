#ifndef SNAKE_DEFINED
#define SNAKE_DEFINED

using namespace std;

class Pit;

class Snake
{
public:
	// Constructor
	Snake(Pit* pp, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;

	// Mutators
	void move();

private:
	Pit* m_pit;
	int  m_row;
	int  m_col;
};

#endif		//SNAKE_DEFINED