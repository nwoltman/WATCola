#include "vending_machine.h"
#include "name_server.h"
   
VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, 
								unsigned int maxStockPerFlavour ) : _prt( prt ), _nameServer( nameServer ), 
								_id( id ), _sodaCost( sodaCost ), _maxStockPerFlavour( maxStockPerFlavour )
{
	_prt.print(Printer::VendingMachine, VendingMachine::Starting);
	_nameServer.VMregister(this);			// register with name server
	for (int i = 0; i < NUM_FLAVOURS; i++) // initally stock is empty
			_stock[i] = 0; 
	_outOfStock = false;
	_insufficientFunds = false;
	_restocking = false;
}
								
void VendingMachine::main() {
	for ( ;; ) {
		
		_Accept ( ~VendingMachine ) {
			break;
		} or _When (!_restocking) _Accept( buy ) {
			_insufficientFunds = ( _card.getBalance() < _sodaCost ) ? true : false;  // check there's enough money
			_outOfStock  = ( stock[_requestedFlavour] == 0 ) ? true : false; // check we have it in stock
				
			if ( !_insufficientFunds && !_outOfStock){ // it is bought, withdraw money and give pop
				_card.withdraw( _sodaCost );
				_stock[_requestedFlavour]--;
			}
			
			_buyBench.signal();
			
		} 
	}
}
 
void VendingMachine::buy( Flavours flavour, WATCard &card ) {
	_card = card;
	_requestedFlavour = flavour;
	
	_buyBench.wait(); // pass it off to the main
	
	if (_insufficientFunds) throw Funds();
	if (_outOfStock) throw Stock();
	
}

unsigned int *VendingMachine::inventory() {
	_restocking = true;
	return _stock;
}

void VendingMachine::restocked() {
	_restocking = false;
}

_Nomutex unsigned int VendingMachine::cost() {
	return _sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
	return _id;
}