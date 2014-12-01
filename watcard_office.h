#ifndef WATCARD_OFFICE_H
#define WATCARD_OFFICE_H

#include "watcard.h"
#include "printer.h"
#include "bank.h"
#include <vector>
#include <queue>

_Task WATCardOffice {
	struct Args {
		unsigned int _sid,
		unsigned int _amount;
		WATCard *_card;
		Args(unsigned int sid, unsigned int amount, WATCard::FWATCard &card) : _sid(sid), _amount(amount), _card(card) {}
	}
	
    struct Job {                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;          // return future
        Job( Args args ) : args( args ) {}
    };
    _Task Courier { // communicates with bank
		unsigned int _id;
		Printer &_prt;
		Bank &_bank;
		WATCardOffice &_watOffice;
	  public:
		Courier( unsigned int id, Printer &prt, Bank &bank, WATCardOffice &watOffice );
	};                 
	
	std::vector<Courier*> _couriers;
	std::queue<Job*> _jobs;
	Job _currentJob;
	Printer &_prt;
	Bank &_bank;
	unsigned int _numCouriers;
    void main();
    
    enum State {
        Starting = 'S', RequestComplete = 'W', CreateComplete = 'C', TransferComplete = 'T', Finished = 'F'
    };
    
  public:
    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
