#include "parent.h"
#include "mprng.h"

Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay )
    : _prt(prt), _bank(bank), _numStudents(numStudents), _parentalDelay(parentalDelay) { }

Parent::~Parent() { }

void Parent::main() {
	_prt.print( Printer::Parent, (char)Parent::Starting ); // Starting task

	for ( ;; ) {
		_Accept(~Parent) break;
		_Else {}

		yield( _parentalDelay ); // delay parentalDelay times

		unsigned int randomMoney = g_mprng( 1, 3 );
		unsigned int randomStudent = g_mprng( _numStudents );

		_bank.deposit( randomStudent, randomMoney );
		_prt.print( Printer::Parent, (char)Parent::Deposit, randomStudent, randomMoney );
	}

	_prt.print( Printer::Parent, (char)Parent::Finished ); // Finished task
}
