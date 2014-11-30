#include "watcard.h"

WATCard::WATCard( const WATCard & ) {}           // prevent copying
WATCard &WATCard::operator=( const WATCard & ) {}

WATCard::WATCard(): _balance(0) {}

void WATCard::deposit( unsigned int amount ) {
	_balance += amount;
}

void WATCard::withdraw( unsigned int amount ) {
	_balance -= amount;
}

unsigned int WATCard::getBalance() {
	return _balance;
}
