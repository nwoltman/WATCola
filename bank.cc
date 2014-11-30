#include "bank.h"

using namespace std;

Bank::Bank( unsigned int numStudents ) : _numStudents(numStudents) {
	for (int i = 0; i < numStudents; i++) {	
		_accounts.insert( pair<unsigned int, unsigned int>(i,0) ); // initialize all student accounts with balance of 0
	}
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
	if (id >= _numStudents) { return; } // make sure its a valid id
	
	_accounts.at(id) += amount;
}

void Bank::withdraw( unsigned int id, unsigned int amount ) { // STILL UNCLEAR TO ME HOW CALLER KNOWS THIS WON'T PRODUCE NEGATIVE VALUE
	if (id >= _numStudents) { return; } // make sure its a valid id
	
	_accounts.at(id) -= amount;
}
