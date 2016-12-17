// Game.cpp

#include "Game.h"
#include "utilities.h"
#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance)
{
	//Create the dungeon
	d = new Dungeon();
	d->setDistance(goblinSmellDistance);
}

void Game::play()
{
	d->displayMap("");
	bool done = false;

	char input = getCharacter();
	while (input != 'q')
	{
		if (!d->gameOver())
		{
			string msg = "";

			//Heal player
			if (trueWithProbability(.1))
				d->player()->increaseHealth(1);

			switch (input)
			{
				case 'h':
					msg += d->player()->move(ARROW_LEFT);
					break;
				case 'l':
					msg += d->player()->move(ARROW_RIGHT);
					break;
				case 'k':
					msg += d->player()->move(ARROW_UP);
					break;
				case 'j':
					msg += d->player()->move(ARROW_DOWN);
					break;
				case 'g':
					msg += d->lootItem();
					break;
				case 'w':
					d->showInventory();
					input = getCharacter();
					msg += d->equipObject(input, "weapon");
					break;
				case 'r':
					d->showInventory();
					input = getCharacter();
					msg += d->equipObject(input, "scroll");
					break;
				case 'i':
					d->showInventory();
					input = getCharacter();
					break;
				case '>':
					if (d->cell(d->player()->row(), d->player()->col()) == '>')
						d->nextDungeonLevel();
					break;
				case 'c':
					d->player()->cheat();
					break;
			}
			//Monsters move after player makes move
			for (int i = 0; i < d->actors().size(); i++)
			{
				if (d->actors().at(i)->name() != "Player")
					msg += d->actors().at(i)->move();
			}
			d->displayMap(msg);
		}

		//When the Player wins or dies
		if (d->gameOver() && !done)
		{
			cout << "Press q to exit game." << endl;
			done = true;
		}
		input = getCharacter();
	}
}

// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.
