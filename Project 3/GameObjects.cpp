#include "GameObjects.h"
#include "utilities.h"
#include <iostream>
#include <ctime>

using namespace std;

void Dungeon::showInventory()
{
	clearScreen();

	cout << "Inventory:" << endl;
	for (size_t i = 0; i < inventory().size(); i++)
	{
		cout << " " << static_cast<char>('a' + i) << ". ";
		if (inventory().at(i)->type() == "scroll")
			cout << "A scroll called ";
		cout << inventory().at(i)->name() << endl;
	}
}
bool Dungeon::isOpen(int row, int col, char subject)
{
	if (gameMap[row][col] == '#')
		return false;

	if (subject == 'a')		//Actor
	{
		for (int i = 0; i < m_actors.size(); i++)
		{
			if (m_actors.at(i)->row() == row && m_actors.at(i)->col() == col)
				return false;
		}
	}
	else if (subject == 'o')					//Object
	{
		for (int i = 0; i < m_gameObject.size(); i++)
		{
			if (m_gameObject.at(i)->row() == row && m_gameObject.at(i)->col() == col)
				return false;
		}
	}
	return true;
}
void Dungeon::randPosition(int &row, int &col, char subject)
{
	//Pick a random room to generate
	int i = rand() % m_rooms.size();

	//Pick random coordinate from the room
	row = m_rooms.at(i)->x1() + (rand() % m_rooms.at(i)->nRows() - 1);
	col = m_rooms.at(i)->y1() + (rand() % m_rooms.at(i)->nCols() - 1);

	//If the coordinate is a wall, we regenerate the random position
	while (!isOpen(row, col, subject))
	{
		row = m_rooms.at(i)->x1() + (rand() % m_rooms.at(i)->nRows() - 1);
		col = m_rooms.at(i)->y1() + (rand() % m_rooms.at(i)->nCols() - 1);
	} 
}
void Dungeon::displayMap(std::string message)
{
	clearScreen();

	//Copy the original game map to a local game map 
	char copyGameMap[MAXROWS][MAXCOLS];
	for (int i = 0; i < MAXROWS; i++)
		for (int j = 0; j < MAXCOLS; j++)
			copyGameMap[i][j] = gameMap[i][j];

	//Add items to the copy of the game map
	for (int i = 0; i < m_gameObject.size(); i++)
		copyGameMap[m_gameObject.at(i)->row()][m_gameObject.at(i)->col()] = m_gameObject.at(i)->symbol();

	//Add actors to the copy of the game map
	for (int i = 0; i < m_actors.size(); i++)
		copyGameMap[m_actors.at(i)->row()][m_actors.at(i)->col()] = m_actors.at(i)->symbol();

	//Display the copy of the game map
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
			cout << copyGameMap[i][j];
		cout << endl;
	}

	cout << "Dungeon Level: " << getLevel() << ", Hit points: " << m_gamer->health()
		<< ", Armor: " << m_gamer->armor() << ", Strength: " << m_gamer->strength()
		<< ", Dexterity: " << m_gamer->dexterity() << endl << endl;
	cout << message;
}
string Dungeon::lootItem()
{
	//Check for when inventory is maxed out
	if (m_inventory.size() > 25)
		return "Your knapsack is full; you can't pick that up.";

	string s = "";

	//Try to find that specific game Object in the gameObject vector and switch to the inventory vector
	for (int i = 0; i < gameObject().size(); i++)
	{
		//When the player is on top of the gameObject
		if (player()->row() == gameObject().at(i)->row() && player()->col() == gameObject().at(i)->col())
		{
			//Pick up game object
			GameObject* temp = gameObject().at(i);
			m_inventory.push_back(temp);
			s = "You pick up ";
			if (gameObject().at(i)->type() == "scroll")
				s += "a scroll called ";
			s += gameObject().at(i)->name();
			s += "\n";

			if (gameObject().at(i)->type() == "golden idol")
			{
				s += "Congratulations, you won!\n";
				m_gameStatus = true;
			}

			//Remove item from the GameObject vector which removes it from the visual
			//say in report that the vector gameobject holds all the game objects
			vector<GameObject*>::iterator it = m_gameObject.begin();
			for (int k = 0; k < i; k++, it++);
			if (it != m_gameObject.end())
				it = m_gameObject.erase(it);
			break;
		}
	}

	//Return the statement when you pick up the item
	return s;
}
string Dungeon::equipObject(char input, string type)
{
	string s = "";
	
	//Get the ASCII Code equivalent of the letter
	int i = input - 97;

	if (i >= 0 && i < m_inventory.size())
	{
		//Equip the weapon to the player
		if (type == "weapon")
		{
			if (m_inventory.at(i)->type() == "weapon")
			{
				m_gamer->equipWeapon(m_inventory.at(i));
				s += "You are wielding " + m_inventory.at(i)->name() + "\n";
			}
			//Fail if try to wield a scroll as a weapon
			else
				return "You can't wield " + m_inventory.at(i)->name() + "\n";
		}
		else		 //scroll
		{
			//Read the scroll 
			if (m_inventory.at(i)->type() == "scroll")
			{
				m_inventory.at(i)->action();
				s += "You read the scroll called " + m_inventory.at(i)->name() + "\n";
				s += m_inventory.at(i)->actionString();
				
				//Remove scroll from the inventory after reading it
				vector<GameObject*>::iterator it = m_inventory.begin();
				for (int k = 0; k < i; k++, it++);
				if (it != m_inventory.end())
					it = m_inventory.erase(it);
			}
		}
	}
	//Return statement after equiping object
	return s;
}
void Dungeon::createMap()
{
	//Generate level with only walls first
	fillMap(0, MAXROWS, 0, MAXCOLS, '#');

	//Generate Rooms on Map
	createRoom();

	//Add Player
	addPlayer();

	//Add Monsters
	addMonsters();

	//Add Items
	addObjects();
}
void Dungeon::fillMap(int x1, int x2, int y1, int y2, char filler)
{
	//Fill the map by row and column with character passed in
	for (int i = x1; i < x2; i++)
		for (int j = y1; j < y2; j++)
			gameMap[i][j] = filler;
}
void Dungeon::createRoom()
{
	//Create rooms
	int numOfRooms = rand() % 2 + 4;

	//Loop through the number of rooms and create them at random spots
	for (int i = 0; i < numOfRooms; i++)
	{
		int x = rand() % (MAXROWS / 2) + 1;
		int nRow = rand() % (MAXROWS / 3) + 4;
		int y = rand() % (abs(i - 5) + 1) + 1;
		int nCol = rand() % (MAXCOLS / (i + 4)) + 4;

		//Shift rooms towards the right from the second one on
		if (i > 0)
			y += m_rooms.at(i - 1)->nCols() + m_rooms.at(i - 1)->y1();

		//If the size of the room goes beyond the outermost walls
		if (x + nRow >= MAXROWS)
			nRow = MAXROWS - x -1;
		if (y + nCol >= MAXCOLS)
			nCol = MAXCOLS - y - 1;
		if (y < MAXCOLS - 4)
		{
			fillMap(x, x + nRow, y, y + nCol, ' ');
			m_rooms.push_back(new Room(x, nRow, y, nCol));
		}
		//If the room goes beyond the boundary, we break out of the loop and just keep that amount of rooms
		else
			break;
	}

	//Connect the rooms with corridors
	addCorridors();
}
void Dungeon::addPlayer()
{
	//Get a random position
	int r, c;
	randPosition(r, c);
	
	//If no player exists yet, which only happens at the initilization of the game
	if (m_gamer == nullptr)
	{
		m_gamer = new Player(r, c, this);

		//Place short sword to the inventory initially
		ShortSword* ss = new ShortSword(0, 0, this);
		m_inventory.push_back(ss);
	}
	else
		//Place player at a random position in the new level
		m_gamer->setPosition(r, c);

	m_actors.push_back(m_gamer);
}
void Dungeon::addMonsters()
{
	//Finding the number of monsters to put on the map
	int nMonsters = 2 + randInt(5 * (getLevel() + 1));

	//Adding the monsters to the map
	for (int i = 0; i < nMonsters; i++)
	{
		//Get a random position for the monster
		int r, c;
		randPosition(r, c);

		//Creating random numbers that will be assigned to each monster
		int monster = rand() % 4;
		switch (monster)
		{
			//Goblins
			case 0:
			{
				Goblin* g = new Goblin(r, c, this);
				m_actors.push_back(g);
				break;
			}
			//Snakewoman
			case 1:
			{
				Snakewomen* s = new Snakewomen(r, c, this);
				m_actors.push_back(s);
				break;
			}
			//Bogeymen
			case 2:
			{
				if (m_dungeonLevel >= 2)
				{
					Bogeymen* b = new Bogeymen(r, c, this);
					m_actors.push_back(b);
					break;
				}
				//In case we are not at the right dungeon level for Bogeymen, 
				//I created a 50/50 chance for a Goblin or Snakewomen to be generated 
				else
				{
					Goblin* g = new Goblin(r, c, this);
					m_actors.push_back(g);
					break;
				}
			}
			//Dragons
			case 3:
			{
				if (m_dungeonLevel >= 3)
				{
					Dragon* d = new Dragon(r, c, this);
					m_actors.push_back(d);
					break;
				}
				else
					//In case we are not at the right dungeon level for Dragon, 
					//I created a 50/50 chance for a Goblin or Snakewomen to be generated 
				{
					Snakewomen* s = new Snakewomen(r, c, this);
					m_actors.push_back(s);
					break;
				}
			}
		}
	}
}
void Dungeon::addObjects()
{
	//Randomize amount of objects on floor initially
	int nObjects = rand() % 2 + 2;

	//Loop to place the objects
	for (int i = 0; i < nObjects; i++)
	{
		//Get a random position for the object
		int r, c;
		randPosition(r, c, 'o');

		int object = randInt(7);
		switch (object)
		{
			//Short Sword
			case 0:
			{
				ShortSword* ss = new ShortSword(r, c, this);
				m_gameObject.push_back(ss);
				break;
			}
			//Mace
			case 1:
			{
				Mace* m = new Mace(r, c, this);
				m_gameObject.push_back(m);
				break;
			}
			//Long Sword
			case 2:
			{
				LongSword* ls = new LongSword(r, c, this);
				m_gameObject.push_back(ls);
				break;
			}
			//Scroll of Armor
			case 3:
			{
				ArmorScroll* as = new ArmorScroll(r, c, this);
				m_gameObject.push_back(as);
				break;
			}
			//Scroll of Strength 
			case 4:
			{
				StrengthScroll* strs = new StrengthScroll(r, c, this);
				m_gameObject.push_back(strs);
				break;
			}
			//Scroll of Health
			case 5:
			{
				HealthScroll* hs = new HealthScroll(r, c, this);
				m_gameObject.push_back(hs);
				break;
			}
			//Scroll of Dexterity
			case 6:
			{
				DexterityScroll* ds = new DexterityScroll(r, c, this);
				m_gameObject.push_back(ds);
				break;
			}
		}
	}

	//Adding the stair charcter to the game map
	if (m_dungeonLevel < 4)
	{
		int row, col;
		randPosition(row, col);
		gameMap[row][col] = '>';
	}

	//Adding golden idol to game map
	if (m_dungeonLevel == 4)
	{
		int row, col;
		randPosition(row, col);

		GoldenIdol* gi = new GoldenIdol(row, col, this);
		m_gameObject.push_back(gi);
	}

}
int Dungeon::intercept(int room1, int room2)
{
	//Intercept at the middle of room1 to anywhere in room2
	int center = m_rooms.at(room1)->centerRow();
	if (center >= m_rooms.at(room2)->x1() && center < m_rooms.at(room2)->x2())
		return center;

	//Intercept at the top of room1 to anywhere in room2
	int top = m_rooms.at(room1)->x1();
	if (top >= m_rooms.at(room2)->x1() && top < m_rooms.at(room2)->x2())
		return top;

	//Intercept at the bottom of room1 to anywhere in room2
	int bottom = m_rooms.at(room1)->x2();
	if (bottom >= m_rooms.at(room2)->x1() && bottom < m_rooms.at(room2)->x2())
		return bottom;

	return -1;
}
void Dungeon::fillCorridors(int rowCol, int begin, int end, char direction)
{
	//Check to see if a corridor can be created with invalid parameters
	if (begin > end)
	{
		int temp = begin;
		begin = end;
		end = temp;
	}

	//Generate corridors horizontally
	if (direction == 'h')
		for (int i = begin; i < end; i++)
			gameMap[rowCol][i] = ' ';

	//Generate corridors vertically
	else if (direction == 'v')
		for(int i = begin; i < end; i++)
			gameMap[i][rowCol] = ' ';
		
}
void Dungeon::addCorridors()
{
	for (int i = 0; i < m_rooms.size() - 1; i++)
	{
		//Check if room intercept with the next room
		int a = intercept(i, i + 1);
		if (a != -1)
			fillCorridors(a, m_rooms.at(i)->y1(), m_rooms.at(i + 1)->y1());

		//From room i + 1 to room i
		a = intercept(i + 1, i);
		if (a != -1)
			fillCorridors(a, m_rooms.at(i + 1)->y1(), m_rooms.at(i)->y1());

		//From room i to i + 2
		if (i < m_rooms.size() - 2)
		{
			a = intercept(i, i + 2);
			if (a != -1)
				fillCorridors(a, m_rooms.at(i)->y1(), m_rooms.at(i + 2)->y1());
		}

		//Create angled corridor
		fillCorridors(m_rooms.at(i + 1)->centerRow(), m_rooms.at(i)->centerCol(), m_rooms.at(i + 1)->y1());
		fillCorridors(m_rooms.at(i)->centerCol(), m_rooms.at(i + 1)->centerRow(), m_rooms.at(i)->x1(), 'v');
	}
}
void Dungeon::nextDungeonLevel()
{
	//Increase the dungeon level when moving down
	m_dungeonLevel += 1;

	//Remove all the rooms
	m_rooms.clear();

	//Remove all the actors except the Player
	vector<Actor*>::iterator it = m_actors.begin();
	for (; it != m_actors.end(); it++)
		if ((*it)->name() != "Player")
			delete * it;
	m_actors.clear();

	//Remove all of the game Objects on the floor
	vector<GameObject*>::iterator q = m_gameObject.begin();
	for (; q != m_gameObject.end(); q++)
		delete *q;
	m_gameObject.clear();

	//Generate a new map
	createMap();
}
void Dungeon::removeActor(int row, int col)
{
	//Delete monster after it has been killed
	vector<Actor*>::iterator it = m_actors.begin();
	for (; it != m_actors.end(); it++)
	{
		if ((*it)->row() == row && (*it)->col() == col)
		{
			delete *it;
			it = m_actors.erase(it);
			break;
		}
	}
}
