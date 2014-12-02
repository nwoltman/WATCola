#include "watcard_office.h"
#include "mprng.h"
#include <uFuture.h>

using namespace std;

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers )
	: _prt( prt ), _bank( bank ), _numCouriers( numCouriers )
{
	_couriers = new WATCardOffice::Courier*[numCouriers];
	for ( unsigned int i = 0; i < numCouriers; i++ ) {
		_couriers[i] = new WATCardOffice::Courier( i, _prt, _bank, *this );
	}
}

WATCardOffice::~WATCardOffice() {
	for ( unsigned int i = 0; i < _numCouriers; i++ ) {
		delete _couriers[i];
	}
	delete[] _couriers;
}

void WATCardOffice::main() {
	 _prt.print( Printer::WATCardOffice, WATCardOffice::Starting );

	for ( ;; ) {

		_Accept ( ~WATCardOffice ) {
			break;
		} or _Accept( create ) {
			_jobs.push( _currentJob ); // add job to queue then let the couriers know
			_courierBench.signal();
		} or _Accept( transfer ) {
			_jobs.push( _currentJob );
			_courierBench.signal();
		}

	}
	_prt.print( Printer::WATCardOffice, WATCardOffice::Finished );
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
	WATCard *card = new WATCard(); // create a new WATCard for the student
	_currentJob = new WATCardOffice::Job( sid, amount, card );
	// this will only work if the _Accept( create ) is guaranteed to run right after

	_prt.print( Printer::WATCardOffice, (char)WATCardOffice::CreateComplete, sid, amount );

	return _currentJob->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
	_currentJob = new WATCardOffice::Job( sid, amount, card );

	_prt.print( Printer::WATCardOffice, (char)WATCardOffice::TransferComplete, sid, amount );

	return _currentJob->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
	if ( _jobs.empty() ) _courierBench.wait();   // Wait until there is a job

	Job* requestedJob = _jobs.front();
	_jobs.pop();

	_prt.print( Printer::WATCardOffice, WATCardOffice::RequestComplete );
	return requestedJob;
}

/*Courier Implementation*/

WATCardOffice::Courier::Courier( unsigned int id, Printer &prt, Bank &bank, WATCardOffice &watOffice ) : _id( id ),
                                _prt( prt ), _bank( bank ), _watOffice( watOffice ) {

}

void WATCardOffice::Courier::main() {
	_prt.print( Printer::Courier, WATCardOffice::Courier::Starting );
	for ( ;; ) {
		_Accept ( ~Courier ) {
			break;
		} _Else { // continue with the rest of the loop
		}

		Job* job = _watOffice.requestWork();

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

