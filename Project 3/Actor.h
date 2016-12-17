#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include <string>

class GameObject;
class Dungeon;

class Actor
{
public:
	Actor(int health, int armor, int strength, int dexterity, int row, int column, char symbol, Dungeon *d) : 
		m_health(health), m_armor(armor), m_strength(strength), m_dexterity(dexterity), m_row(row), 
		m_col(column), m_symbol(symbol), m_dungeon(d) {};
	~Actor(){};
	int row(){ return m_row; }		//return the row where the actor is
	int col() { return m_col; }		//return the column where the actor is
	char symbol(){ return m_symbol; }		//return the symbol of the actor
	int health() { return m_health; }		//return the health of the actor
	int armor() { return m_armor; }			//return the armor of the actor
	int strength() { return m_strength; }	//return the strength of the actor
	int dexterity() { return m_dexterity; }	//return the dexterity of the actor
	void damage(int damagePoints){ m_health -= damagePoints; }	//change the health of actor after taking damage
	virtual std::string attack(int row, int col);		//attack actor if they are next to the attacking actor
	std::string name() { return m_name; }				//return name of the actor such as "Player" or "Dragon"
	virtual std::string move(char direction = ' ');		//move the actor in a certain direction
	virtual void drop() {}								//function to drop item after killing monster
	virtual void increaseHealth(int incHealth) { m_health += incHealth; }	//increase health of actor
	//Recursive function for Goblin and generally monsters
	virtual bool pathExists(int mr, int mc, int pr, int pc, int distance, int direction){ return true; }
	//Supplement to recursive function to actually make a move
	void moveOneStep(int row, int col);

protected:
	GameObject* m_weapon;
	Dungeon* m_dungeon;
	int m_row;
	int m_col;
	int m_health;
	int m_maxhealth;
	int m_strength;
	int m_armor;
	int m_dexterity;
	std::string m_name;

private:
	char m_symbol;

};
class Player : public Actor
{
public:
	Player(int row, int column, Dungeon *d);
	~Player();
	virtual std::string move(char direction);						//Player's ability to move
	virtual std::string attack(int row, int col);					//Player's abiilty to attack a monster next to it
	void equipWeapon(GameObject* weapon) { m_weapon = weapon; }		//Equip new weapon from inventory
	void improveArmor(int armor)									//Improve armor after reading scroll
	{ 
		m_armor += armor; 
		if (m_armor >= 99)
			m_armor = 99;
	}
	void enhanceDex(int dexterity)									//Improve dexterity after reading scroll
	{ 
		m_dexterity += dexterity; 
		if (m_dexterity >= 99)
			m_dexterity = 99;
	}
	void raiseStrength(int strength)								//Improve strength after reading scroll
	{ 
		m_strength += strength; 
		if (m_strength >= 99)
			m_strength = 99;
	}
	void increaseMaxHealth(int health) { m_health += health; }		//Increase the max health after reading a scroll
	void setPosition(int row, int col) { m_row = row; m_col = col; }//Set position of the actor in map
	virtual void increaseHealth(int incHealth)						//Increase health after moving
	{ 
		m_health += incHealth; 
		if (m_health > m_maxhealth)
			m_health = m_maxhealth;
	}
	void cheat(){ m_maxhealth = 50; m_strength = 9; m_health = 50; }//Cheat in the game to make it easier to win

private:

};
class Goblin : public Actor
{
public:
	Goblin(int row, int column, Dungeon *d);
	~Goblin();
	virtual void drop();					//If killed, drop something based on probability
	bool pathExists(int mr, int mc, int pr, int pc, int distance, int direction);	//Recursive function to move Goblin
	
};
class Snakewomen : public Actor
{
public:
	Snakewomen(int row, int column, Dungeon *d);
	~Snakewomen();
	virtual bool pathExists(int mr, int mc, int pr, int pc, int distance, int direction);	//Function to move Snakewomen
	virtual void drop();		//If killed, drop something based on probability
};
class Bogeymen : public Actor
{
public:
	Bogeymen(int row, int column, Dungeon *d);
	~Bogeymen();
	virtual bool pathExists(int mr, int mc, int pr, int pc, int distance, int direction);	//Function to move Bogeymen	
	virtual void drop();		//If killed, drop something based on probability
};
class Dragon : public Actor
{
public:
	Dragon(int row, int column, Dungeon *d);
	~Dragon();
	virtual std::string move(char direction);		//Function to move Bogeymen	
	virtual void drop();		//If killed, drop something based on probability
	virtual void increaseHealth(int incHealth) { m_health += incHealth; }	//Increase health after turns randomly
};
#endif	//ACTOR_INCLUDED