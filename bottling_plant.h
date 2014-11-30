#ifndef BOTTLING_PLANT_H
#define BOTTLING_PLANT_H

#include "printer.h"
#include "name_server.h"

#define NUM_FLAVOURS 4

_Task BottlingPlant {
	Printer &_prt;
	NameServer &_nameServer;
	unsigned int _numVendingMachines;
	unsigned int _maxShippedPerFlavour;
	unsigned int _maxStockPerFlavour;
	unsigned int _timeBetweenShipments;
	unsigned int _productionRun[NUM_FLAVOURS];
	bool _closingDown;
	
	enum State {
        Starting = 'S', Generating = 'G', PickedUp = 'P', Finished = 'F'
    };
	
    void main();
  public:
    _Event Shutdown {};                    // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );
};

#endif
