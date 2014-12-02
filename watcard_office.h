#ifndef WATCARD_OFFICE_H
#define WATCARD_OFFICE_H

#include "watcard.h"
#include "printer.h"
#include "bank.h"
#include <queue>

_Task WATCardOffice {
    struct Job {                           // marshalled arguments and return future
        unsigned int _sid;                 // student ID
        unsigned int _amount;              // amount to put on card
        WATCard *_card;                    // pointer to student's card
        WATCard::FWATCard result;          // return future
        Job( unsigned int sid, unsigned int amount, WATCard *card )
            : _sid( sid ), _amount( amount ), _card( card ) {}
    };

    _Task Courier { // communicates with bank
		unsigned int _id;
		Printer &_prt;
		Bank &_bank;
		WATCardOffice &_watOffice;
		void main();

		enum State {
			Starting = 'S', TransferStart = 't', TransferComplete = 'T', Finished = 'F'
		};
	  public:
		Courier( unsigned int id, Printer &prt, Bank &bank, WATCardOffice &watOffice );
	};

	Courier **_couriers;
	std::queue<Job*> _jobs;
	Job *_currentJob;
	Printer &_prt;
	Bank &_bank;
	unsigned int _numCouriers;
	uCondition _courierBench;
    void main();

    enum State {
        Starting = 'S', RequestComplete = 'W', CreateComplete = 'C', TransferComplete = 'T', Finished = 'F'
    };

  public:
    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
