#include "watcard_office.h"
#include "MPRNG.h"

using namespace std;

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) : _prt( prt ), _bank( bank ), 
                             _numCouriers( numCouriers ) 
{
	for ( unsigned int i = 0; i < _numCouriers; i++ ) {
		couriers.push_back( new WATCardOffice::Courier( i, _prt, _bank, *this ) );
	}
}

void WATCardOffice::main() {
	 _prt.print( Printer::WATCardOffice, WATCardOffice::Starting );
	
	for ( ;; ) {

		_Accept ( ~WATCardOffice ) {
			break;
		} or _Accept( create ) {
			_jobs.push(_currentJob );
		} or _Accept( transfer ) {
			_jobs.push( _currentJob );
		} 
			
	}
	_prt.print( Printer::WATCardOffice, WATCardOffice::Finished );
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
	_currentJob = new Job(Args(sid, amount, NULL)); // this will only work is the _Accept( create ) is guaranteed to run right after
	
	return _currentJob->result; 
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
	_currentJob = new Job(Args(sid, amount, card));
	
	return _currentJob->result;
}

Job *WATCardOffice::requestWork() {
	if ( _jobs.empty )
		return NULL;   		// I'M SURE A BETTER WAY WILL BECOME OBVIOUS WHEN COURIER IS IMPLEMENTED
	Job* requestedJob = _jobs.front();
	_jobs.pop();
	return requestedJob;
}

/*Courier Implementation*/

WATCardOffice::Courier::Courier( unsigned int id, Printer &prt, Bank &bank, WATCardOffice &watOffice ) : _id( id ), 
                                _prt( prt ), _bank( bank ), _watOffice( watOffice ) {
	
}
