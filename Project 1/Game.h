#ifndef GAME_DEFINED
#define GAME_DEFINED

using namespace std;

class Pit;

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nSnakes);
	~Game();

	// Mutators
	void play();

private:
	Pit* m_pit;
};

#endif		//GAME_DEFINED