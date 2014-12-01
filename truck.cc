#include "truck.h"
#include "vending_machine.h"


Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
              unsigned int numVendingMachines, unsigned int maxStockPerFlavour )
    : _printer( prt ), _nameServer( nameServer ), _plant( plant ),
      _numVendingMachines( numVendingMachines ), _maxStockPerFlavour( maxStockPerFlavour ) { }

unsigned int Truck::countSodas( const unsigned int flavours[] ) {
    unsigned int numSodas = 0;
    for ( unsigned int i = 0; VendingMachine::NumFlavours; i++ ) {
        numSodas += cargo[i];
    }
    return numSodas;
}

void Truck::main() {
    _printer.print( Printer::Truck, Truck::Starting );               // Print starting state

    VendingMachine **vendingMachines = _nameServer.getMachineList(); // Get the list of vending machines
    unsigned int cargo[VendingMachine::NumFlavours];                 // Initialize soda cargo
    for ( unsigned int i = 0; i < VendingMachine::NumFlavours; i++ ) {
        cargo[i] = 0;
    }
    unsigned int firstMachine = 0;                                   // Remember the first machine to get a shipment

    for ( ;; ) {
        try {
            yield( 1, 10 );                                          // Get coffee at Tim Hortons
            _plant.getShipment( cargo );                             // Pick up shipment of soda

            unsigned int shipmentSize = countSodas( cargo );
            _printer.print( Printer::Truck, Truck::PickedUp, shipmentSize ); // Print picked up shipment
            if ( shipmentSize == 0 ) continue;                       // If no soda was picked up, try again
        }
        catch ( BottlingPlant::Shutdown ) {
            break;
        }

        for ( unsigned int i = 0; i < _numVendingMachines; i++ ) {
            unsigned int machine = (firstMachine + i) % _numVendingMachines;
            _printer.print( Printer::Truck, Truck::BeginDelivery, machine, countSodas( cargo ) ); // Begin delivery
            unsigned int *inventory = vendingMachines[machine]->inventory();
            unsigned int numMissing = 0;                             // The number of sodas not replenished

            for ( unsigned int i = 0; i < VendingMachine::NumFlavours; i++ ) { // Fill each flavour
                unsigned int curStock = inventory[i];
              if ( curStock == _maxStockPerFlavour ) continue;       // Nothing to do if stock is full
                unsigned int needed = _maxStockPerFlavour - curStock;
                unsigned int canGive = cargo[i];
                if ( needed > canGive ) {                            // Fill as much stock as possible
                    inventory[i] += canGive;
                    cargo[i] = 0;
                    numMissing += needed - canGive;
                } else {                                             // Fill the stock completely
                    inventory[i] = _maxStockPerFlavour;
                    cargo[i] -= needed;
                }
            }

            if ( numMissing > 0 ) {                                  // Print if machine wasn't fully restocked
                _printer.print( Printer::Truck, Truck::UnsuccessfulFilling, machine, numMissing );
            }

            unsigned int numSodasLeft = countSodas( cargo );
            _printer.print( Printer::Truck, Truck::EndDelivery, machine, numSodasLeft ); // End delivery
            if ( numSodasLeft == 0 ) break;                          // If cargo is empty, get more soda
        }

        firstMachine = (firstMachine + 1) % _numVendingMachines;     // Update which machine gets the first shipment
    }

    _printer.print( Printer::Truck, Truck::Finished );               // Print finshed state
}
