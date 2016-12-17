#ifndef SEQUENCE_INCLUDED
#define SEQUENCE_INCLUDED

#include <string>

typedef std::string ItemType;

class Sequence
{
public:
	Sequence();
	bool empty() const;
	int size() const;
	bool insert(int pos, const ItemType& value);
	bool insert(const ItemType& value);
	bool erase(int pos);
	int remove(const ItemType& value);
	bool get(int pos, ItemType& value) const;
	bool set(int pos, const ItemType& value);
	int find(const ItemType& value) const;
	void swap(Sequence& other);
	void dump();

	//Destructor
	~Sequence();

	//Copy Constructor
	Sequence(const Sequence& other);

	//Assignment Operator
	Sequence& operator=(const Sequence& rhs);

private:
	struct Node
	{
		ItemType m_data;
		Node* m_next;
		Node* m_prev;
	};
	void insertFirstNode(const ItemType& value);
	void insertOtherNode(Node* p, const ItemType& value);
	Node* m_head;
	int m_size;
};

//Public Functions
int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);

//Inline Implementations
inline
int Sequence::size() const
{
	return m_size;
};

inline
bool Sequence::empty() const
{
	return size() == 0;
};

#endif // SEQUENCE_INCLUDED