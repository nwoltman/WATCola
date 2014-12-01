#include "bottling_plant.h"
#include "mprng.h"
#include "truck.h"

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                              unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                              unsigned int timeBetweenShipments )
    : _prt ( prt ),
      _nameServer( nameServer ),
      _numVendingMachines( numVendingMachines ),
      _maxShippedPerFlavour( maxShippedPerFlavour ),
      _maxStockPerFlavour( maxStockPerFlavour ),
      _timeBetweenShipments( timeBetweenShipments ),
      _closingDown( false )
{
	for ( int i = 0; i < VendingMachine::NumFlavours; i++ ) {
		_productionRun[i] = 0;
    }
}

BottlingPlant::~BottlingPlant() {}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
	if ( _closingDown ) {
		uRendezvousAcceptor();
		throw Shutdown();
	}

	for ( int i = 0; i < VendingMachine::NumFlavours; i++ )
		cargo[i] = _productionRun[i];  // Fill cargo with produced soda (may be 0)
}

void BottlingPlant::main() {
	_prt.print( Printer::BottlingPlant, BottlingPlant::Starting );

	Truck truck( _prt, _nameServer, *this, _numVendingMachines, _maxStockPerFlavour ); // create truck

	for ( ;; ) {
		_Accept( ~BottlingPlant ) {
			_closingDown = true;
			_Accept( getShipment );
			break;
		} _Else { // continue with the rest of the loop
        }

		yield( _timeBetweenShipments );                           // yield to simulate production time

		int totalBottles = 0;
		for ( unsigned int i = 0; i < VendingMachine::NumFlavours; i++ ) {
			_productionRun[i] = g_mprng( _maxShippedPerFlavour ); // new production run
			totalBottles += (int)_productionRun[i];               // count total bottles for printer
		}
		_prt.print( Printer::BottlingPlant, BottlingPlant::Generating, totalBottles );

		_Accept( getShipment ); //wait for truck to pick up shipment
		_prt.print( Printer::BottlingPlant, BottlingPlant::PickedUp );
	}

	_prt.print( Printer::BottlingPlant, BottlingPlant::Finished );
}
