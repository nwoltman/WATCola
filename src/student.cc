#include "student.h"
#include "mprng.h"
#include "vending_machine.h"
#include "watcard.h"


Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
                  unsigned int maxPurchases )
    : _printer( prt ), _nameServer( nameServer ), _cardOffice( cardOffice ), _id( id ),
      _maxPurchases( maxPurchases )
{ }

void Student::main() {
    // Randomly choose a number of sodas to buy [0, _maxPurchases]
    unsigned int numToBuy = g_mprng( _maxPurchases );
    // Randomly choose a favourite flavour of soda [0, VendingMachine::NumFlavours - 1]
    unsigned int favouriteFlavour = g_mprng( VendingMachine::NumFlavours - 1 );

    _printer.print( Printer::Student, _id, (char)Student::Starting, favouriteFlavour, numToBuy ); // Starting

    WATCard *card = NULL;                                       // Will be set to the actual card later
    WATCard::FWATCard fcard = _cardOffice.create( _id, 5 );     // Create a WATCard with a balance of $5
    VendingMachine *vmachine = _nameServer.getMachine( _id );   // Select an initial vending machine
    _printer.print( Printer::Student, _id, (char)Student::SelectedVM, vmachine->getId() );

    while ( numToBuy > 0 ) {                                    // Loop while there are still sodas left to buy
        yield( g_mprng( 1, 10 ) );                              // Before buying a soda, yield randomly [1, 10]

        for ( ;; ) {                                            // Inifinite loop to try and buy a soda
            try {                                               // Try to buy a single soda
                if ( card == NULL ) {                           // Retrieve the card if it is still in the future
                    card = fcard();
                }
                vmachine->buy( (VendingMachine::Flavours)favouriteFlavour, *card );
                break;                                          // Done buying one soda
            }
            catch ( WATCardOffice::Lost ) {                     // Handle lost card exception
                _printer.print( Printer::Student, _id, (char)Student::LostCard );
                fcard = _cardOffice.create( _id, 5 );           // Create a new WATCard with a balance of $5
                card = NULL;
            }
            catch ( VendingMachine::Funds ) {                   // Handle insufficient funds exception
                unsigned int amount = vmachine->cost() + 5;
                fcard = _cardOffice.transfer( _id, amount, card ); // Transfer more funds to card
                card = NULL;
            }
            catch ( VendingMachine::Stock ) {                   // Handle out of stock exception
                vmachine = _nameServer.getMachine( _id );       // Go to a new vending machine
                _printer.print( Printer::Student, _id, (char)Student::SelectedVM, vmachine->getId() );
            }
        }
                                                                // Successfully bought a soda
        _printer.print( Printer::Student, _id, (char)Student::BoughtSoda, card->getBalance() );
        numToBuy -= 1;                                          // Decrement the number of sodas left to buy
    }

    if ( card != NULL ) {                                       // Student must delete their card
        delete card;
    }

    _printer.print( Printer::Student, _id, (char)Student::Finished ); // Finished
}
