#ifndef STUDENT_H
#define STUDENT_H

#include "name_server.h"
#include "printer.h"
#include "watcard_office.h"

_Task Student {
    enum States { Starting = 'S', SelectedVM = 'V', BoughtSoda = 'B', LostCard = 'L', Finished = 'F' };
    Printer &_printer;
    NameServer &_nameServer;
    WATCardOffice &_cardOffice;
    unsigned int _id;
    unsigned int _maxPurchases;
    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
             unsigned int maxPurchases );
};

#endif
