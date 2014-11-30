#include "watcard.h"

    WATCard::WATCard( const WATCard & ) {}           // prevent copying
    WATCard::WATCard &operator=( const WATCard & ) {}
 
    WATCard(): balance(0) {}
	
    void WATCard::deposit( unsigned int amount ) {
		balance += amount;
	}
	
    void WATCard::withdraw( unsigned int amount ) {
		balance -= amount;
	}
	
    unsigned int WATCard::getBalance() {
		return balance;
	}
