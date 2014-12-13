#include "watcard_office.h"
#include "mprng.h"
#include <uFuture.h>
using namespace std;


WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers )
	: _prt( prt ), _bank( bank ), _numCouriers( numCouriers ) { }

WATCardOffice::~WATCardOffice() { }

void WATCardOffice::main() {
	 _prt.print( Printer::WATCardOffice, WATCardOffice::Starting );

	 // Create the couriers
	WATCardOffice::Courier *couriers[_numCouriers];
	for ( unsigned int i = 0; i < _numCouriers; i++ ) {
		couriers[i] = new WATCardOffice::Courier( i, _prt, _bank, *this );
	}

    // Main loop
	for ( ;; ) {
		_Accept ( ~WATCardOffice ) {
			break;
		} or _Accept( create, transfer ) {
			_jobs.push( _currentJob );
		} or _When( _jobs.size() > 0 ) _Accept( requestWork ) {
			_jobs.pop();
		}
	}

	for ( unsigned int i = 0; i < _numCouriers; i++ ) { // Give the couriers null jobs so that they will finish
		_jobs.push( NULL );
	}
	while ( _jobs.size() > 0 ) {                        // Wait until all the couriers are done
		_Accept( requestWork );
	}
	for ( unsigned int i = 0; i < _numCouriers; i++ ) { // Delete the couriers
		delete couriers[i];
	}

	_prt.print( Printer::WATCardOffice, WATCardOffice::Finished );
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
	_currentJob = new WATCardOffice::Job( sid, amount, new WATCard() );
	_prt.print( Printer::WATCardOffice, (char)WATCardOffice::CreateComplete, sid, amount );
	return _currentJob->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
	_currentJob = new WATCardOffice::Job( sid, amount, card );
	_prt.print( Printer::WATCardOffice, (char)WATCardOffice::TransferComplete, sid, amount );
	return _currentJob->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
	_prt.print( Printer::WATCardOffice, WATCardOffice::RequestComplete );
	return _jobs.front();
}


/* Courier Implementation */

WATCardOffice::Courier::Courier( unsigned int id, Printer &prt, Bank &bank, WATCardOffice &watOffice )
	: _id( id ), _prt( prt ), _bank( bank ), _watOffice( watOffice ) { }

void WATCardOffice::Courier::main() {
	_prt.print( Printer::Courier, WATCardOffice::Courier::Starting );

	for ( ;; ) {
		Job* job = _watOffice.requestWork();

		if ( job == NULL ) break; // Getting a null job means there are no jobs left to do, so the courier can end

		_prt.print( Printer::Courier, (char)WATCardOffice::Courier::TransferStart, job->_sid, job->_amount );

		_bank.withdraw( job->_sid, job->_amount);
		job->_card->deposit( job->_amount );

		if ( g_mprng( 5 ) == 3 ) { // simulate 1 in 6 chance of losing card
			delete job->_card;
			job->result.exception( new Lost() );
		} else {
			job->result.delivery( job->_card );
		}

		_prt.print( Printer::Courier, (char)WATCardOffice::Courier::TransferComplete, job->_sid, job->_amount );

		delete job;
	}

	_prt.print( Printer::Courier, WATCardOffice::Courier::Finished );
}

