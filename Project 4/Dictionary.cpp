// Dictionary.cpp

// This is a correct but horribly inefficient implementation of Dictionary
// functionality.  Your job is to change DictionaryImpl to a more efficient
// correct implementation.  You can change it any way you like, subject to
// restrictions in the spec (e.g., that the only C++ library container you
// are allowed to use are vector, list, stack, and queue (and string); if you
// want anything fancier, implement it yourself).

#include "Dictionary.h"
#include <string>
#include <vector>
#include <list>
#include <cctype>
#include <algorithm>  // for swap
using namespace std;

void removeNonLetters(string& s);
void generateNextPermutation(string& permutation);

// This class does the real work of the implementation.

const int MAXBUCKETS = 50000;

class DictionaryImpl
{
public:
	DictionaryImpl() {}
	~DictionaryImpl() {}
	void insert(string word);
	void lookup(string letters, void callback(string)) const;
private:
	struct Node
	{
		string m_signature;
		vector<string> *m_anagrams;
	};
	vector<Node*> m_dictionary[MAXBUCKETS];
};

//Hashing Function: RS Hash algorithm from the internet - http://stackoverflow.com/questions/6136964/rs-hashing-program
unsigned int RSHash(const string& str)
{
	unsigned int a = 2166136261U;
	unsigned int b = 16777619;
	unsigned int hash = 0;

	for (std::size_t i = 0; i < str.length(); i++)
	{
		hash = hash * a + str[i];
		a = a * b;
	}

	return hash;
}
/* End Of RS Hash Function */

void DictionaryImpl::insert(string word)
{
	removeNonLetters(word);
	if (!word.empty())
	{
		//Sort the letters of the string into alphabetical order, which becomes the key
		string localKey = word;
		std::sort(localKey.begin(), localKey.end());

		//Call hashing function to translate key into an index of one of the buckets - get correct terminology
		unsigned int hash = RSHash(localKey) % MAXBUCKETS;

		//Insert into dictionary
		if (m_dictionary[hash].size() == 0)		//No entries in the bucket
		{
			Node *p = new Node;
			p->m_signature = localKey;
			p->m_anagrams = new vector < string > ;
			p->m_anagrams->push_back(word);
			m_dictionary[hash].push_back(p);
			return;
		}
		//If there is a key already existant at that hash
		else
		{
			for (int i = 0; i < m_dictionary[hash].size(); i++)
			{
				//If the key exists at that hash, push into anagram
				if (localKey == m_dictionary[hash].at(i)->m_signature)
				{
					m_dictionary[hash].at(i)->m_anagrams->push_back(word);
					return;
				}
			}

			//The key does not exist, so create new Node
			Node *p = new Node;
			p->m_signature = localKey;
			p->m_anagrams = new vector < string >;
			p->m_anagrams->push_back(word);
			m_dictionary[hash].push_back(p);
			return;
		}
	}
}

void DictionaryImpl::lookup(string letters, void callback(string)) const
{
	if (callback == nullptr)
		return;

	removeNonLetters(letters);
	if (letters.empty())
		return;

	string localLetters = letters;
	std::sort(localLetters.begin(), localLetters.end());
	unsigned int hash = RSHash(localLetters) % MAXBUCKETS;

	if (m_dictionary[hash].size() > 0)
	{
		for (int i = 0; i < m_dictionary[hash].size(); i++)
		{
			if (m_dictionary[hash].at(i)->m_signature == localLetters)
			{
				for (int j = 0; j < m_dictionary[hash].at(i)->m_anagrams->size(); j++)
					callback(m_dictionary[hash].at(i)->m_anagrams->at(j));
			}
		}
	}
}

void removeNonLetters(string& s)
{
	string::iterator to = s.begin();
	for (string::const_iterator from = s.begin(); from != s.end(); from++)
	{
		if (isalpha(*from))
		{
			*to = tolower(*from);
			to++;
		}
	}
	s.erase(to, s.end());  // chop off everything from "to" to end.
}

// Each successive call to this function generates the next permutation of the
// characters in the parameter.  For example,
//    string s = "eel";
//    generateNextPermutation(s);  // now s == "ele"
//    generateNextPermutation(s);  // now s == "lee"
//    generateNextPermutation(s);  // now s == "eel"
// You don't have to understand this function's implementation.
void generateNextPermutation(string& permutation)
{
	string::iterator last = permutation.end() - 1;
	string::iterator p;

	for (p = last; p != permutation.begin() && *p <= *(p - 1); p--)
		;
	if (p != permutation.begin())
	{
		string::iterator q;
		for (q = p + 1; q <= last && *q > *(p - 1); q++)
			;
		swap(*(p - 1), *(q - 1));
	}
	for (; p < last; p++, last--)
		swap(*p, *last);
}

//******************** Dictionary functions **********************************

// These functions simply delegate to DictionaryImpl's functions.
// You probably don't want to change any of this code.

Dictionary::Dictionary()
{
	m_impl = new DictionaryImpl;
}
Dictionary::~Dictionary()
{
	delete m_impl;
}
void Dictionary::insert(string word)
{
	m_impl->insert(word);
}
void Dictionary::lookup(string letters, void callback(string)) const
{
	m_impl->lookup(letters, callback);
}
