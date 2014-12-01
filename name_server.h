#ifndef NAME_SERVER_H
#define NAME_SERVER_H

#include "printer.h"
#include "vending_machine.h"


_Task NameServer {
    enum States { Starting = 'S', Register = 'R', NewMachine = 'N', Finished = 'F' };
    Printer &_printer;
    unsigned int _numVendingMachines;
    unsigned int _numStudents;
    VendingMachine **_vendingMachines;
    VendingMachine *_machineToRegister;
    unsigned int *_studentMachineIdMap;
    unsigned int _studentId;
    void main();
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif
