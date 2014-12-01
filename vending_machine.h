#ifndef VENDING_MACHINE_H
#define VENDING_MACHINE_H

#include "printer.h"
#include "watcard.h"

_Task NameServer;

_Task VendingMachine {
	Printer &_prt;
	NameServer &_nameServer;
	unsigned int _id;
	unsigned int _sodaCost;
	unsigned int _maxStockPerFlavour;
	
	WATCard *_card;
	
	
	uCondition _buyBench;
	bool _outOfStock;
	bool _insufficientFunds;
	bool _restocking;
	
    void main();
    enum State {
        Starting = 'S', StartReloading = 'r', CompleteReloading = 'R', SodaBought = 'B', Finished = 'F'
    };
  public:
    enum Flavours { BlackCherry = 0, CreamSoda = 1, RootBeer = 2, Lime = 3, NUM_FLAVOURS = 4};             // flavours of soda
    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
    
  private:
	Flavours _requestedFlavour;
	unsigned int _stock[ NUM_FLAVOURS ]; // 0 => BlackCherry, 1 => CreamSoda, 2 => RootBeer, 3 => Lime
};

#endif
