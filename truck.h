#ifndef TRUCK_H
#define TRUCK_H

#include "bottling_plant.h"
#include "name_server.h"
#include "printer.h"


_Task Truck {
    enum State {
        Starting = 'S', PickedUp = 'P', BeginDelivery = 'd',
        UnsuccessfulFilling = 'U', EndDelivery = 'D', Finished = 'F'
    };
    Printer &_printer;
    NameServer &_nameServer;
    BottlingPlant &_plant;
    unsigned int _numVendingMachines;
    unsigned int _maxStockPerFlavour;
    unsigned int countSodas( const unsigned int flavours[] ); // Counts the number of sodas in the array
    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif
