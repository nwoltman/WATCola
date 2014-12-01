#include "name_server.h"


NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents )
    : _printer( prt ), _numVendingMachines( numVendingMachines ), _numStudents( numStudents )
{
    _vendingMachines = new VendingMachine*[numVendingMachines];
    _studentMachineIdMap = new unsigned int[numStudents];

    // Distribute students evenly among the vending machines
    for ( unsigned int i = 0; i < numStudents; i += 1 ) {
        _studentMachineIdMap[i] = i % numVendingMachines;
    }
}

NameServer::~NameServer() {
    delete[] _vendingMachines;
    delete[] _studentMachineIdMap;
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
    _machineToRegister = vendingmachine;
}

VendingMachine *NameServer::getMachine( unsigned int id ) {
    _studentId = id;                                              // Record which student just got a machine
    unsigned int machineId = _studentMachineIdMap[id];            // Get the student's vending machine ID
    return _vendingMachines[machineId];                           // Retrieve and return the vending machine
}

VendingMachine **NameServer::getMachineList() {
    return _vendingMachines;
}

void NameServer::main() {
    _printer.print( Printer::NameServer, NameServer::Starting );  // Starting task

    for ( unsigned int i = 0; i < _numVendingMachines; i += 1 ) { // Wait until all machines are registered
        _Accept( VMregister );                                    // Block until someone calls VMregister
        _printer.print( Printer::NameServer, (char)NameServer::Register, _machineToRegister->getId() );
        _vendingMachines[i] = _machineToRegister;
    }

    for ( ;; ) {
        _Accept( ~NameServer ) {                                  // Terminate?
            break;
        } or _Accept( getMachine ) {                              // Update the student's vending machine ID
            unsigned int machineId = _studentMachineIdMap[_studentId];
            // Print the vending machine the student just got
            _printer.print( Printer::NameServer, (char)NameServer::NewMachine, _studentId, machineId );
            // Round-robin to the next vending machine
            _studentMachineIdMap[_studentId] = (machineId + 1) % _numVendingMachines;
        }
    }

    _printer.print( Printer::NameServer, NameServer::Finished );  // Finished task
}
