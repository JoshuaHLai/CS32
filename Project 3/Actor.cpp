#include "Actor.h"
#include "GameObjects.h"
#include "utilities.h"

using namespace std;

string Actor::attack(int row, int col)
{
	//Attack for the monsters
	string s;

	int attackerPoints = dexterity() + m_weapon->dexBonus();
	int defenderPoints = m_dungeon->player()->dexterity() + m_dungeon->player()->armor();
	s += name() + m_weapon->actionString() + m_dungeon->player()->name();

	//Monster hits player
	if (randInt(attackerPoints) >= randInt(defenderPoints))
	{
		int damagePoints = randInt(strength() + m_weapon->weaponDamage());
		m_dungeon->player()->damage(damagePoints);

		//If the players health goes below 0, end game
		if (m_dungeon->player()->health() <= 0)
		{
			s += " dealing a final blow.\n";
			m_dungeon->setGameStatus(true);
		}
		else
			s += " and hits.\n";

		//If hit with magic fangs of sleep
	}
	else
		s += " and misses.\n";

	return s;
}
Player::Player(int row, int column, Dungeon *d) : Actor(20, 2, 2, 2, row, column, '@', d)
{ m_weapon = new ShortSword(0, 0, d); m_name = "Player";  m_maxhealth = 20; }
string Player::move(char direction)
{
	//Print out action if any happen
	string s;

	switch (direction)
	{
		case ARROW_LEFT:
			if (m_dungeon->isOpen(m_row, m_col - 1))
				m_col--;
			else
				//Attack a monster and print out statement
				s += m_dungeon->player()->attack(m_row, m_col - 1);
			break;
		case ARROW_RIGHT:
			if (m_dungeon->isOpen(m_row, m_col + 1))
				m_col++;
			else
				//Attack a monster and print out statement
				s += m_dungeon->player()->attack(m_row, m_col + 1);
			break;
		case ARROW_UP:
			if (m_dungeon->isOpen(m_row - 1, m_col))
				m_row--;
			else
				//Attack a monster and print out statement
				s += m_dungeon->player()->attack(m_row - 1, m_col);
			break;
		case ARROW_DOWN:
			if (m_dungeon->isOpen(m_row + 1, m_col))
				m_row++;
			else
				//Attack a monster and print out statement
				s += m_dungeon->player()->attack(m_row + 1, m_col);
			break;
	}
	return s;
}
string Player::attack(int row, int col)
{
	string s;

	//Find out if the spot next to the player is a monster
	for (int i = 0; i < m_dungeon->actors().size(); i++)
	{
		if (m_dungeon->actors().at(i)->row() == row && m_dungeon->actors().at(i)->col() == col)
		{
			int attackerPoints = dexterity() + m_weapon->dexBonus();
			int defenderPoints = m_dungeon->actors().at(i)->dexterity() + m_dungeon->actors().at(i)->armor();
			s += name() + m_weapon->actionString() + m_dungeon->actors().at(i)->name();

			//Player has hit the monster
			if (randInt(attackerPoints) >= randInt(defenderPoints))
			{
				int damagePoints = randInt(strength() + m_weapon->weaponDamage());
				m_dungeon->actors().at(i)->damage(damagePoints);
				
				//If the monsters health goes below 0, delete monster
				if (m_dungeon->actors().at(i)->health() <= 0)
				{
					if (m_dungeon->isOpen(row, col, 'o'))
						m_dungeon->actors().at(i)->drop();

					m_dungeon->removeActor(row, col);
					s += " dealing a final blow.\n";
				}
				else
					s += " and hits.\n";

				//If hit with magic fangs of sleep

			}
			else
				s += " and misses.\n";
		}
	}
	return s;
}
Goblin::Goblin(int row, int column, Dungeon *d) : Actor(rand() % 6 + 15, 1, 3, 1, row, column, 'G', d)
{ m_weapon = new ShortSword(0, 0, d); m_name = "the Goblin"; }
string Actor::move(char direction)
{
	//This function is not complete.  The most I was able to do on this function was to have the Goblins move
	//towards the Player at a large distance.  I was also only able to get it close enough to make it where the
	//Player had to be relatively in the same row and column of the Goblin.  The Goblins are able to chase the 
	//Player but will stop when there is something that minutely blocks their direct path to the Player.
	
	string s;

	//Save the player's coordinates to local variables to use throughout the function
	int row = m_dungeon->player()->row();
	int col = m_dungeon->player()->col();

	//If player is next to monster, then attack
	if ((row == m_row + 1 && col == m_col) || (row == m_row && col == m_col - 1) ||
		(row == m_row - 1 && col == m_col) || (row == m_row && col == m_col + 1))
		s += attack(row, col);
	else		//the monster will move towards the player
	{
		//Player is on the same row as the Goblin
		if (row == m_row)
		{
			//Monster will move to the left
			if (col < m_col)
			{
				//Check if monster can move to that spot
				if (pathExists(m_row, m_col, row, col, 0, ARROW_LEFT))
					moveOneStep(row, col);	//move to that spot that is also closer to the Player
			}
			//Monster will move to the right
			else
			{
				if (pathExists(m_row, m_col, row, col, 0, ARROW_RIGHT))
					moveOneStep(row, col);
			}
		}
		//Player is on the same column as the Goblin
		else if (col == m_col)
		{
			//Monster will move up
			if (row < m_row)
			{
				if (pathExists(m_row, m_col, row, col, 0, ARROW_UP))
					moveOneStep(row, col);
			}
			//Monster will move down
			else
			{
				if (pathExists(m_row, m_col, row, col, 0, ARROW_DOWN))
					moveOneStep(row, col);
			}
		}
		//Player is below the Monster
		else if (row > m_row)
		{
			if (col < m_col)	//Player is to the left of the Monster
			{
				if (pathExists(m_row, m_col, row, col, 0, ARROW_LEFT) && pathExists(m_row, m_col, row, col, 0, ARROW_DOWN))
					moveOneStep(row, col);
			}
			else				//Player is to the right of Monster
			{
				if (pathExists(m_row, m_col, row, col, 0, ARROW_RIGHT) && pathExists(m_row, m_col, row, col, 0, ARROW_DOWN))
					moveOneStep(row, col);
			}
		}
		//Player is above the Monster
		else if (row < m_row)
		{
			if (col < m_col)	//Player is to the left of the Monster
			{
				if (pathExists(m_row, m_col, row, col, 0, ARROW_LEFT) && pathExists(m_row, m_col, row, col, 0, ARROW_UP))
					moveOneStep(row, col);
			}
			else				//Player is to the right of Monster
			{
				if (pathExists(m_row, m_col, row, col, 0, ARROW_RIGHT) && pathExists(m_row, m_col, row, col, 0, ARROW_UP))
					moveOneStep(row, col);
			}
		}
	}
	return s;
}
void Goblin::drop()
{
	//There is a 1 in 3 chance that a Goblin will drop something when killed
	if (trueWithProbability(.33))
	{
		//Randomly choose between the two items that are dropped
		switch (randInt(2))
		{
			case 0:
			{	
				MagicAxe* ma = new MagicAxe(row(), col(), m_dungeon);
				m_dungeon->addObject(ma);
				break;
			}
			case 1:
			{
				MagicFang* mf = new MagicFang(row(), col(), m_dungeon);
				m_dungeon->addObject(mf);
				break;
			}
		}
	}
}
bool Goblin::pathExists(int mr, int mc, int pr, int pc, int distance, int direction)
{
	//End the call if the distance passed in is greater than 
	//the distance used when initializing the game
	if (distance > m_dungeon->getDistance())
		return false;
	
	//When the Player is next to Goblin, end the recursive call
	if ((direction == ARROW_LEFT) || (direction == ARROW_RIGHT) && (mc == pc))
		return true;
	if ((direction == ARROW_UP) || (direction == ARROW_DOWN) && (mr == pr))
		return true;
	
	//Based on the direction, tell the Goblin to move in that direction
	switch (direction)
	{
	case ARROW_UP:
		if (pathExists(mr - 1, mc, pr, pc, distance + 1, direction))
			return true;
		break;
	case ARROW_DOWN:
		if (pathExists(mr + 1, mc, pr, pc, distance + 1, direction))
			return true;
		break;
	case ARROW_LEFT:
		if (pathExists(mr, mc - 1, pr, pc, distance + 1, direction))
			return true;
		break;
	case ARROW_RIGHT:
		if (pathExists(mr, mc + 1, pr, pc, distance + 1, direction))
			return true;
		break;
	}

	return false;
}
void Actor::moveOneStep(int row, int col)
{
	bool moved = false;
	
	//Player is left of the Goblin
	if (col < m_col)
	{
		if (m_dungeon->isOpen(m_row, m_col - 1))
		{
			m_col--;
			moved = true;
		}
	}
	//Player is to the right of the Goblin
	else
	{
		if (m_dungeon->isOpen(m_row, m_col + 1))
		{
			m_col++;
			moved = true;
		}
	}

	if (!moved)
	{
		//Player is above the Goblin
		if (row < m_row)
		{
			if (m_dungeon->isOpen(m_row - 1, m_col))
				m_row--;
		}
		//Player is below the Goblin
		else
			if (m_dungeon->isOpen(m_row + 1, m_col))
				m_row++;
	}
}
Snakewomen::Snakewomen(int row, int column, Dungeon *d) : Actor(rand() % 4 + 3, 3, 2, 3, row, column, 'S', d)
{ m_weapon = new MagicFang(0, 0, d); m_name = "the Snakewomen"; }
bool Snakewomen::pathExists(int mr, int mc, int pr, int pc, int distance, int direction)
{
	//End the call if the distance passed in is greater than 3
	if (distance > 3)
		return false;

	//When the Player is next to Snakewomen, end the recursive call
	if ((direction == ARROW_LEFT) || (direction == ARROW_RIGHT) && (mc == pc))
		return true;
	if ((direction == ARROW_UP) || (direction == ARROW_DOWN) && (mr == pr))
		return true;

	//Based on the direction, tell the Snakewomen to move in that direction
	switch (direction)
	{
	case ARROW_UP:
		if (pathExists(mr - 1, mc, pr, pc, distance + 1, direction))
			return true;
		break;
	case ARROW_DOWN:
		if (pathExists(mr + 1, mc, pr, pc, distance + 1, direction))
			return true;
		break;
	case ARROW_LEFT:
		if (pathExists(mr, mc - 1, pr, pc, distance + 1, direction))
			return true;
		break;
	case ARROW_RIGHT:
		if (pathExists(mr, mc + 1, pr, pc, distance + 1, direction))
			return true;
		break;
	}

	return false;
}
void Snakewomen::drop()
{
	//Drop magic fangs of sleep based on probability
	if (trueWithProbability(.33))
	{
		MagicFang* mf = new MagicFang(row(), col(), m_dungeon);
		m_dungeon->addObject(mf);
	}
}
Bogeymen::Bogeymen(int row, int column, Dungeon *d) 
	: Actor(rand() % 6 + 5, 2, rand() % 2 + 2, rand() % 2 + 2, row, column, 'B', d)
{ m_weapon = new ShortSword(0, 0, d); m_name = "the Bogeymen"; }
bool Bogeymen::pathExists(int mr, int mc, int pr, int pc, int distance, int direction)
{
	//End the call if the distance passed in is greater than 5
	if (distance > 5)
		return false;

	//When the Player is next to Goblin, end the recursive call
	if ((direction == ARROW_LEFT) || (direction == ARROW_RIGHT) && (mc == pc))
		return true;
	if ((direction == ARROW_UP) || (direction == ARROW_DOWN) && (mr == pr))
		return true;

	//Based on the direction, tell the Goblin to move in that direction
	switch (direction)
	{
	case ARROW_UP:
		if (pathExists(mr - 1, mc, pr, pc, distance + 1, direction))
			return true;
		break;
	case ARROW_DOWN:
		if (pathExists(mr + 1, mc, pr, pc, distance + 1, direction))
			return true;
		break;
	case ARROW_LEFT:
		if (pathExists(mr, mc - 1, pr, pc, distance + 1, direction))
			return true;
		break;
	case ARROW_RIGHT:
		if (pathExists(mr, mc + 1, pr, pc, distance + 1, direction))
			return true;
		break;
	}

	return false;
}
void Bogeymen::drop()
{
	//Drop item based on probability
	if (trueWithProbability(.1))
	{
		MagicAxe* ma = new MagicAxe(row(), col(), m_dungeon);
		m_dungeon->addObject(ma);
	}
}
Dragon::Dragon(int row, int column, Dungeon *d) : Actor(rand() % 6 + 20, 4, 4, 4, row, column, 'D', d)
{ m_weapon = new LongSword(0, 0, d); m_name = "the Dragon"; }
string Dragon::move(char direction)
{
	string s;

	int row = m_dungeon->player()->row();
	int col = m_dungeon->player()->col();

	//If player is next to monster, then attack
	if ((row = m_row + 1 && col == m_col) || (row = m_row && col == m_col - 1) ||
		(row = m_row - 1 && col == m_col) || (row = m_row && col == m_col + 1))
		s += attack(row, col);

	//Recover health after turn based on probability
	if (trueWithProbability(.1))
		increaseHealth(1);

	return s;
}
void Dragon::drop()
{
	//Drop scroll based on which number is randomly generated
	switch (randInt(5))
	{
		case 0:
		{
			ArmorScroll* as = new ArmorScroll(row(), col(), m_dungeon);
			m_dungeon->addObject(as);
			break;
		}
		case 1:
		{
			StrengthScroll* strs = new StrengthScroll(row(), col(), m_dungeon);
			m_dungeon->addObject(strs);
			break;
		}
		case 2:
		{
			HealthScroll* hs = new HealthScroll(row(), col(), m_dungeon);
			m_dungeon->addObject(hs);
			break;
		}
		case 3:
		{
			DexterityScroll* ds = new DexterityScroll(row(), col(), m_dungeon);
			m_dungeon->addObject(ds);
			break;
		}
		case 4:
		{
			TeleporationScroll* ts = new TeleporationScroll(row(), col(), m_dungeon);
			m_dungeon->addObject(ts);
			break;
		}
		}
}
