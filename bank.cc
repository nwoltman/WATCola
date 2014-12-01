#include "bank.h"
using namespace std;


Bank::Bank( unsigned int numStudents ) : _numStudents( numStudents ) {
    _accounts = new unsigned int[numStudents];
    for ( unsigned int i = 0; i < numStudents; i++ ) {
        _accounts[i] = 0; // initialize all student accounts with balance of 0
    }
}

Bank::~Bank() {
    delete[] _accounts;
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    _accounts[id] += amount;
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    while ( _accounts[id] < amount ) _Accept( deposit ); // Wait until the account has enough money
    _accounts[id] -= amount;
}
