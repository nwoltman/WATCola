#include "vending_machine.h"
#include "name_server.h"


VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
								unsigned int maxStockPerFlavour )
	: _prt( prt ), _nameServer( nameServer ), _id( id ), _sodaCost( sodaCost ),
	  _maxStockPerFlavour( maxStockPerFlavour )
{
	_prt.print( Printer::Vending, id, (char)VendingMachine::Starting, sodaCost );

	for ( int i = 0; i < VendingMachine::NumFlavours; i++ ) // initally stock is empty
			_stock[i] = 0;

	_nameServer.VMregister( this );			// register with name server
	_outOfStock = false;
	_insufficientFunds = false;
	_restocking = false;
}

void VendingMachine::main() {
	for ( ;; ) {
		_Accept ( ~VendingMachine ) {
			break;
		} or _When ( !_restocking ) _Accept( buy ) {
			_insufficientFunds = _card->getBalance() < _sodaCost;  // check if the card has enough money
			_outOfStock  = _stock[_requestedFlavour] == 0;         // check we have it in stock

			if ( !_insufficientFunds && !_outOfStock ) {           // it is bought, withdraw money and give soda
				_card->withdraw( _sodaCost );
				_stock[_requestedFlavour]--;
                _prt.print( Printer::Vending, _id, (char)VendingMachine::SodaBought,
                            _requestedFlavour, _stock[_requestedFlavour] );
			}

			_buyBench.signalBlock();                               // Let the buy member finish

		} or _Accept( inventory, restocked ) {
		}
	}
	_prt.print( Printer::Vending, _id, (char)VendingMachine::Finished );
}

void VendingMachine::buy( Flavours flavour, WATCard &card ) {
	_card = &card;
	_requestedFlavour = flavour;

	_buyBench.wait(); // pass it off to the main

	if ( _insufficientFunds ) throw Funds();
	if ( _outOfStock ) throw Stock();
}

unsigned int *VendingMachine::inventory() {
	_restocking = true;
	_prt.print( Printer::Vending, _id, (char)VendingMachine::StartReloading );
	return _stock;
}

void VendingMachine::restocked() {
	_restocking = false;
	_prt.print( Printer::Vending, _id, (char)VendingMachine::CompleteReloading );
}

_Nomutex unsigned int VendingMachine::cost() {
	return _sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
	return _id;
}
