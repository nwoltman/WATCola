#ifndef BANK_H
#define BANK_H

#include <map>
#include <utility>

_Monitor Bank {
	std::map<unsigned int, unsigned int> _accounts; // <unsigned int id, unsigned int balance>
	unsigned int _numStudents; // not really necessary, used for error checking instead of calling _accounts.size()
	
  public:
    Bank( unsigned int numStudents );
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif
