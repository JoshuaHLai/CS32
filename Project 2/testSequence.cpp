#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Sequence s;
	assert(s.insert(0, 10));
	assert(s.insert(0, 20));
	assert(s.size() == 2);
	ItemType x = 999;
	assert(s.get(0, x) && x == 20);
	assert(s.get(1, x) && x == 10);
}

int main()
{
	test();
	cout << "Passed all tests" << endl;
}


#include <iostream>

#include "Sequence.h"

using namespace std;

int main()
{
	Sequence s;			//new Sequence s constructed
	s.insert(0, 23);		//insert 100 at position 0
	s.insert(1, 300);		//insert 300 at position 1
	s.insert(2, 3000);		//insert 1000 at position 2
	s.insert(3, 2000);		//insert 3000 at position 3
	s.insert(4, 20);		//insert 200 at position 4
	s.insert(5, 70);		//insert 5 at position 5

	Sequence s1;			//new Sequence s1 constructed
	s1.insert(0, 3000);		//insert 3000 at position 0
	s1.insert(1, 200);		//insert 200 at position 1
	s1.insert(2, 20);		//insert 20 at position 2

	Sequence result;
	interleave(s, s1, result);		//should correctly sort numbers into following order:






//	s = s1;
//
//	s.dump();
//	s1.dump();
//	//Sequence result;
//	//cout << endl << "Position is equal to " << subsequence(s, s1) << endl;
//	
//	/*interleave(s, s1, result);*/
//
//	//result.dump();
//
//	/*Sequence s1(s);
//	s1.dump();*/
//
//	/*cout << s.find(800) << endl;
//	cout << s.remove(30) << endl;*/
//	//s.dump();
//
//	/*s.erase(20);
//	cout << endl;
//	s.dump();
//	cout << endl;
//	unsigned long test;
//	cout << s.get(1, test) << endl;
//	cout << s.set(1, 42) << endl;
//	s.dump();*/
//}