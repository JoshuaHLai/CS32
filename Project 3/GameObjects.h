#ifndef GAMEOBJECTS_DEFINED
#define GAMEOBJECTS_DEFINED

#include "Actor.h"
#include <string>
#include <vector>

const int MAXROWS = 18;
const int MAXCOLS = 70;

class Room
{
public:
	Room(int x1, int nRow, int y1, int nCol) : m_x(x1), m_nRow(nRow), m_y(y1), m_nCol(nCol) {}
	~Room(){}
	int x1() { return m_x; }						//Return the first row of the room
	int x2() { return m_x + m_nRow; }				//Return the last row of the room
	int nRows() { return m_nRow; }					//Return the number of rows the room is
	int y1() { return m_y; }						//Return the first column of the room
	int y2() { return m_y + m_nCol; }				//Return the last column of the room
	int nCols() { return m_nCol; }					//Return the number of columns the room is
	int centerRow() { return m_x + (m_nRow / 2); }	//Find the center row of the room to create a corridor
	int centerCol() { return m_y + (m_nCol / 2); }	//Find the center column of the room to create a corridor
private:
	int m_x;
	int m_nRow;
	int m_y;
	int m_nCol;
};
class Dungeon
{
public:
	Dungeon() { createMap(); }
	~Dungeon();

	//Accessor
	void displayMap(std::string message);					//Display the map in the executable
	Player* player() { return m_gamer; }					//Function to return the player
	std::vector<Actor*> actors() { return m_actors; }		//Function to return an actor from the vector
	std::vector<GameObject*> gameObject() { return m_gameObject; }	//Function to return a gameObject from the vector
	std::vector<GameObject*> inventory() { return m_inventory; }	//Function to return an item from the vector inventory
	void showInventory();									//Displays the inventory when called
	bool isOpen(int row, int col, char subject = 'a');	//Default of isOpen is for an Actor
	int getLevel() { return m_dungeonLevel; }				//Checks what the dungeon level is and updates when necessary
	void nextDungeonLevel();								//Creates a new map after descending deeper into the dungeon
	char cell(int row, int col) { return gameMap[row][col]; }	//Returns the position of an actor or gameObject
	bool gameOver() { return m_gameStatus; }				//Checks to see if the game is over or not
	void removeActor(int row, int col);						//Delete a monster after it has been killed
	void setGameStatus(bool status){ m_gameStatus = status; }	//Checks to see if player has died or won
	void addObject(GameObject* item){ m_gameObject.push_back(item); }	//Add object to vector and display on game Map
	void randPosition(int &row, int &column, char subject = 'a');		//Default for randPosition is an Actor
	void setDistance(int distance) { m_goblinSmellDistance = distance; }	//Set the distance passed in from the main.cpp
	int getDistance() { return m_goblinSmellDistance; }					//Find the distance passed in from the main.cpp
	

	//Mutators
	std::string lootItem();									//Pick up an item 
	std::string equipObject(char input, std::string type);	//Equip a weapon or use a scroll
	void addCorridors();									//Add corridors to the map to connect rooms
	void fillCorridors(int rowCol, int begin, int end, char direction = 'h');		//Create the corridors with the char ' '
	int intercept(int room1, int room2);					//Find where the corridors could connect based on the map

private:
	//Mutator
	void createMap();											//Generate the map
	void fillMap(int x1, int x2, int y1, int y2, char filler);	//Fill the map with the character passed in
	void createRoom();											//Create a room
	void addPlayer();											//Add a Player to the game
	void addMonsters();											//Add monsters to the game
	void addObjects();											//Add weapons and scrolls to the game

	std::vector<Actor*> m_actors;
	std::vector<Room*> m_rooms;
	std::vector<GameObject*> m_gameObject;
	std::vector<GameObject*> m_inventory;
	char gameMap[MAXROWS][MAXCOLS];
	int m_dungeonLevel = 0;
	int m_goblinSmellDistance;
	bool m_gameStatus = false;
	Player* m_gamer;
};
class GameObject
{
public:
	GameObject(std::string name, int row, int col, std::string type, Dungeon *d) : m_name(name), m_row(row), 
		m_col(col), m_type(type), m_dungeon(d) {}
	std::string name() { return m_name; }						//Return the name of the object
	std::string type() { return m_type; }						//Return whether the object is a weapon or scorll
	std::string actionString() { return m_actionString; }		//Return what action the object does such as read or wield
	int row() { return m_row; }									//Return the row of the object
	int col() { return m_col; }									//Return the column of the object
	char symbol() { return m_symbol; }							//Return the symbol of the object on the game Map
	int weaponDamage() { return m_damageAmount; }				//Return how much damage the weapon will do after the formula
	int dexBonus() { return m_dexBonus; }						//Return the dexterity bonus of every weapon
	virtual void action(){}										//Return what the object does
protected:
	Dungeon* m_dungeon;
	std::string m_name;
	std::string m_actionString;
	int m_damageAmount;
	int m_strengthBonus;
	int m_dexBonus;
	char m_symbol;
private:
	int m_row;
	int m_col;
	std::string m_type;
	
};
class ShortSword : public GameObject
{
public:
	ShortSword(int row, int col, Dungeon *d) : GameObject("short sword", row, col, "weapon", d) 
	{ m_damageAmount = 2; m_dexBonus = 0; m_symbol = ')'; m_actionString = " slashes short sword at "; }
};
class Mace : public GameObject
{
public:
	Mace(int row, int col, Dungeon *d) : GameObject("mace", row, col, "weapon", d)
	{ m_damageAmount = 2; m_dexBonus = 0; m_symbol = ')'; m_actionString = " swings mace at "; }
};
class LongSword : public GameObject
{
public:
	LongSword(int row, int col, Dungeon *d) : GameObject("long sword", row, col, "weapon", d)
	{ m_damageAmount = 4; m_dexBonus = 2; m_symbol = ')'; m_actionString = " swings long sword at "; }
};
class MagicAxe : public GameObject
{
public:
	MagicAxe(int row, int col, Dungeon *d) : GameObject("magic axe", row, col, "weapon", d)
	{ m_damageAmount = 5; m_dexBonus = 5; m_symbol = ')'; m_actionString = " chops magic axe at "; }
};
class MagicFang : public GameObject
{
public:
	MagicFang(int row, int col, Dungeon *d) : GameObject("magic fangs of sleep", row, col, "weapon", d)
	{ m_damageAmount = 2; m_dexBonus = 3; m_symbol = ')'; m_actionString = " strikes magic fangs at "; }
};
class ArmorScroll : public GameObject
{
public:
	ArmorScroll(int row, int col, Dungeon *d) : GameObject("a scroll of enhance armor", row, col, "scroll", d)
	{ m_symbol = '?'; m_actionString = "Your armor glows blue."; }
	virtual void action() { m_dungeon->player()->improveArmor(rand() % 3 + 1); }	//Improve the armor of player
};
class StrengthScroll : public GameObject
{
public:
	StrengthScroll(int row, int col, Dungeon *d) : GameObject("a scroll of raise strength", row, col, "scroll", d)
	{ m_symbol = '?'; m_actionString = "Your muscles bulge."; }
	virtual void action() { m_dungeon->player()->raiseStrength(rand() % 3 + 1); }		//Improve strength of player
};
class HealthScroll : public GameObject
{
public:
	HealthScroll(int row, int col, Dungeon *d) : GameObject("a scroll of enhance health", row, col, "scroll", d)
	{
		m_symbol = '?'; m_actionString = "You feel your heart beating stronger.";
	}
	virtual void action() { m_dungeon->player()->increaseMaxHealth(rand() % 6 + 3); }		//Increase the max health of player
};
class DexterityScroll : public GameObject
{
public:
	DexterityScroll(int row, int col, Dungeon *d) : GameObject("a scroll of enhance dexterity", row, col, "scroll", d)
	{ m_symbol = '?'; m_actionString = "You feel like less of a klutz."; }
	virtual void action() { m_dungeon->player()->enhanceDex(1); }			//Increse the dexterity of player
};	
class TeleporationScroll : public GameObject
{
public:
	TeleporationScroll(int row, int col, Dungeon *d) : GameObject("a scroll of teleportation", row, col, "scroll", d)
	{
		m_symbol = '?'; m_actionString = "You feel your body wrenched in space and time.";
	}
	virtual void action()								//Teleport player to random spot on map that is open
	{ 
		int row, col; 
		m_dungeon->randPosition(row, col);
		m_dungeon->player()->setPosition(row, col);
	}
};
class GoldenIdol : public GameObject
{
public:
	GoldenIdol(int row, int col, Dungeon *d) : GameObject("golden idol", row, col, "golden idol", d)
	{ m_symbol = '&'; }
};
#endif	//GAMEOBJECTS_DEFINED