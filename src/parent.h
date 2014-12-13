#ifndef PARENT_H
#define PARENT_H

#include "printer.h"
#include "bank.h"

_Task Parent {
    enum States { Starting = 'S', Deposit = 'D', Finished = 'F' };
    Printer &_prt;
    Bank &_bank;
    unsigned int _numStudents;
    unsigned int _parentalDelay;
    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
    ~Parent();
};

#endif
