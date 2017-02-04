// list::merge
#include <iostream>
#include <list>

int main ()
{
/*
	std::list<double> first, *second;
	second = new std::list<double>();
	first.push_back (3.1);
	second->push_back (3.7);

	first.merge(*second);
	// (second is now empty)
	std::cout << "first contains:";
	for (std::list<double>::iterator it=first.begin(); it!=first.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';
	return 0;
*/
	std::list<double> *first, *second;
	first = new std::list<double>();
	second = new std::list<double>();
	first->push_back (3.1);
	second->push_back (3.7);

	first->merge(*second);
	// (second is now empty)
	std::cout << "first contains:";
	for (std::list<double>::iterator it=first->begin(); it!=first->end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';
	return 0;
}
