// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "GameObjects.h"

// You may add data members and other member functions to this class.

class Game
{
public:
	Game(int goblinSmellDistance);
	void play();
private:
	Dungeon* d;
};

#endif // GAME_INCLUDED