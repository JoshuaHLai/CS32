#include <iostream>
#include <cstdlib>
#include "Sequence.h"

//Doubly-Linked Lists
Sequence::Sequence() : m_size(0), m_head(new Node)
{};

bool Sequence::insert(int pos, const ItemType& value)
{
	//Check if the position is in bounds
	if (pos < 0 || pos > size())
		return false;

	//If the Node is the first one inserted
	if (size() == 0)
		insertFirstNode(value);

	//If the Node is in another part of the list
	else
	{
		//Dummy Node
		Node* p = m_head->m_next;

		//Loop to find the position
		for (int i = 0; i < pos ; i++, p = p->m_next)
			;
		insertOtherNode(p, value);
	}

	return true;
};

bool Sequence::insert(const ItemType& value)
{
	//If the Node inserted the first one
	if (size() == 0)
		insertFirstNode(value);
	
	//If the Node is not the first one in the Sequence
	else
	{
		//Create the dummy pointer to loop through the list
		Node* p;
		//Loop to find the position
		for (p = m_head->m_next; p != m_head, p->m_data < value; p = p->m_next)
			;
		insertOtherNode(p, value);
	}

	return true;
};

bool Sequence::erase(int pos)
{
	//Check to see if the position is in the list
	if (pos < 0 || pos >= size())
		return false;

	//Create a dummy pointer
	Node* p = m_head->m_next;

	//Check to make sure size is greater than 0
	if (size() > 1)
	{
		int i;

		//Loop to find the position
		for (i = 0; i < pos; i++, p = p->m_next)
			;

		//Change the pointers to skip the the Node removed
		p->m_prev->m_next = p->m_next;
		p->m_next->m_prev = p->m_prev;
	}
	
	//Delete the Node and decrement the size
	delete p;
	m_size--;
	
	return true;
};

int Sequence::remove(const ItemType& value)
{
	//Check to see if the value is in the list
	if (find(value) == -1)
		return 0;
	
	int itemsRemoved = 0;

	//Create dummy pointers
	Node* p = m_head->m_next;
	Node* q;

	int i;

	//Loop to find the values that need to be removed
	for (i = 0; i < size(); i++, p = p->m_next)
		if (p->m_data == value)
		{
			//Adjust the pointers to make it skip the pointer that needs to be deleted
			p->m_prev->m_next = p->m_next;
			p->m_next->m_prev = p->m_prev;
			
			//Set the dummy pointers equal to each other
			q = p;

			//Shift the original pointer to the next Node
			p = p->m_next;
			
			//Delete the Node
			delete q;
			itemsRemoved++;
		}

	//Edit the size of the List
	m_size = m_size - itemsRemoved;
	return itemsRemoved;
};

bool Sequence::get(int pos, ItemType& value) const
{
	//Check to make sure the position is in the list
	if (pos < 0 || pos >= size())
		return false;

	//Create dummy pointer
	Node* p = m_head->m_next;

	//Loop to find the position in the list
	int i;
	for (i = 0; i < pos; i++, p = p->m_next)
		;
	//Get the value from the Node and set it to the passed in value
	value = p->m_data;
	
	return true;
};

bool Sequence::set(int pos, const ItemType& value)
{
	//Check to see if the position is in the list
	if (pos < 0 || pos >= size())
		return false;

	//Create dummy pointer
	Node* p = m_head->m_next;

	//Loop to find the position
	int i;
	for (i = 0; i < pos; i++, p = p->m_next)
		;

	//Change the value in the Node to the value passed in
	p->m_data = value;

	return true;
};

int Sequence::find(const ItemType& value) const
{
	//Create the dummy Node
	Node* p = m_head->m_next;
	int i;

	//Loop to find the value in the list
	for (i = 0; i < size(); i++, p = p->m_next)
		if (p->m_data == value)
			return i;

	//If the value isn't found
	return -1;
};

void Sequence::swap(Sequence& other)
{
	//Create dummy pointers
	Node* p = m_head->m_next;
	Node* q = m_head->m_prev;
	
	//Assign sizes and record them
	int thisSize = m_size;
	int otherSize = other.m_size;

	//If the list of the size passed in is greater than 0
	if (otherSize > 0)
	{
		//Link the beginning m_head to other beginning Node
		m_head->m_next = other.m_head->m_next;
		other.m_head->m_next->m_prev = m_head;

		//Link the end of other to m_head beginning Node
		other.m_head->m_prev->m_next = m_head;
		m_head->m_prev = other.m_head->m_prev;
	}

	else
		//Append the leftover elements into the swap
		*this = other;

	//If the list of the new list is greater than 0
	if (thisSize > 0)
	{
		//Link the beginning of other to m_head (old list) beginning Node
		other.m_head->m_next = p;
		p->m_prev = other.m_head;

		//Link the end of m_head (old list) to other beginning Node
		q->m_next = other.m_head;
		other.m_head->m_prev = q;
	}
	else
		//Append the leftover elements into the swap
		other = *this;

	//Switch the sizes
	m_size = otherSize;
	other.m_size = thisSize;
};

//Destructor
Sequence::~Sequence()
{
	//Make sure there are still Nodes in the list
	if (size() > 0)
	{
		//Create dummy pointers
		Node* p;
		Node* q;

		//Loop through the list and delete the Nodes
		for (p = m_head->m_next; p != m_head;)
		{
			q = p;
			p = p->m_next;
			delete q;
		}
	}
};

//Copy Constructor
Sequence::Sequence(const Sequence& other) : m_size(0), m_head(new Node)
{
	//Create dummy pointers
	Node* p;
	
	//Create counter
	int i;

	//Loop through the Sequence and insert the values into the new Sequence
	for (i = 0, p = other.m_head->m_next; i < other.m_size, p != other.m_head; i++, p = p->m_next)
		insert(i, p->m_data);
};

//Assignment Operator
Sequence& Sequence::operator=(const Sequence& rhs)
{
	//Copy values into new Sequence
	if (this != &rhs)
	{
		Sequence temp(rhs);
		swap(temp);
	}
	return *this;
};

//New Functions
int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	//Check to see that there is something in the first Sequence
	if (seq1.size() == 0)
		return -1;

	//Check to see that there is something in the second Sequence
	if (seq2.size() == 0)
		return -1;

	//Get the items in sequence 2
	ItemType item, item2;
	seq2.get(0, item);

	//Find the item in sequence 1
	int pos = seq1.find(item);

	//Check if the first item exists in seq1
	if (pos == -1)
		return -1;
	
	int i = pos;
	int j = 0;

	//Loop until the end of the size of sequence 1
	while (i < seq1.size())
	{
		//Get the items in both sequence and check to see that there are equal
		seq1.get(i, item);
		seq2.get(j, item2);

		//Check to see that the items are equal
		if (item == item2)
		{
			if (j == seq2.size() - 1)	//Found the sequence; easy and happy path
				return i - seq2.size() + 1;
			i++;
			j++;
		}
		else
		{
			//Loop until finding another match
			for (; i < seq1.size(); i++)
			{
				//Get the items and compare
				seq1.get(i, item);
				seq2.get(0, item2);
				//Check to see that the items are equal
				if (item == item2)
					break;
			}
			j = 0;
		}
	}

	return -1;
};

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
	//Loop until the end of the either Sequence
	for (int i = 0; i < seq1.size() && i < seq2.size(); i++)
	{
		ItemType dummy;

		//Get the item in sequence 1 and add it to result
		seq1.get(i, dummy);
		result.insert(2 * i, dummy);

		//Get the item in sequence 2 and add it to result
		seq2.get(i, dummy);
		result.insert((2 * i) + 1, dummy);
	}

	//Check to see if there are extra elements
	//in the list if they are not equal size
	if (seq1.size() > seq2.size())
	{
		//Loop until the end of Sequence 1
		for (int i = seq2.size(); i < seq1.size(); i++)
		{
			//Insert the leftover elements into result
			ItemType dummy;
			seq1.get(i, dummy);
			result.insert(i + 2, dummy);
		}
	}
	else if (seq2.size() > seq1.size())
	{
		//Loop until the end of Sequence 2
		for (int i = seq1.size(); i < seq2.size(); i++)
		{
			//Insert the leftover elements into result
			ItemType dummy;
			seq2.get(i, dummy);
			result.insert(i + 2, dummy);
		}
	}
};

//Personal Functions
void Sequence::dump()
{
	//Print out Sequence after using functions
	std::cerr << "=== Dumping List === List Size = " << size() << std::endl;
	if (m_size > 0)
	{
		for (Node* p = m_head->m_next; p != m_head; p = p->m_next)
			std::cerr << p->m_data << std::endl;
	}
};

void Sequence::insertFirstNode(const ItemType& value)
{
	//Create new Node and assign value
	Node* newNode = new Node;
	newNode->m_data = value;

	//Change pointers to insert new Node into the beginning of the empty list
	newNode->m_next = m_head;
	newNode->m_prev = m_head;
	m_head->m_next = newNode;
	m_head->m_prev = newNode;
	m_size++;
};

void Sequence::insertOtherNode(Node* p, const ItemType& value)
{
	//Create new Node and assign value
	Node* newNode = new Node;
	newNode->m_data = value;

	//Change pointers to insert new Node into the list 
	newNode->m_next = p->m_prev->m_next;
	newNode->m_prev = p->m_prev;
	p->m_prev->m_next = newNode;
	p->m_prev = newNode;
	m_size++;
};
