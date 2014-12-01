#include <cstdlib>            // atoll
#include <iostream>
#include <fstream>
using namespace std;
#include "bank.h"
#include "bottling_plant.h"
#include "config.h"
#include "mprng.h"
#include "name_server.h"
#include "parent.h"
#include "printer.h"
#include "student.h"
#include "watcard_office.h"

MPRNG g_mprng;  // Initialize the global MPRNG


// Determines if a char array is made up of only characters that form a positive integer
bool isPosInt( char *c ) {
    // Make sure the string isn't all '0's
    while ( *c == '0' ) c++;
    if ( *c == '\0' ) return false;

    do {
        if ( *c < '0' || '9' < *c ) return false;
        c++;
    } while ( *c != '\0' );

    return true;
}

void printUsageAndExit(char *name) {
    cout << "Usage: " << name << " [ config-file [ random-seed (> 0) ] ]" << endl;
    exit( EXIT_FAILURE );
}


void uMain::main() {
    ConfigParms cparms;
    unsigned int seed = getpid();

    switch ( argc ) {
      case 3:
        if ( ! isPosInt( argv[2] ) ) printUsageAndExit( argv[0] );
        seed = atoll( argv[2] );
      case 2:
        processConfigFile( argv[1], cparms );            // Parse the specified config file
        break;
      case 1:
        processConfigFile( "soda.config", cparms );      // Parse the default config file
        break;
      default:
        printUsageAndExit( argv[0] );
    }

    g_mprng.seed( seed );                                // Seed the MPRNG

    Printer printer( cparms.numStudents, cparms.numVendingMachines, cparms.numCouriers );
    { // Block forces printer to be deleted last
        // Create the name server
        NameServer nameServer( printer, cparms.numVendingMachines, cparms.numStudents );

        // Create the vending machines
        VendingMachine* vendingMachines[ cparms.numVendingMachines ];
        for ( unsigned int i = 0; i < cparms.numVendingMachines; i += 1 ) {
            vendingMachines[i] = new VendingMachine( printer, nameServer, i, cparms.sodaCost,
                                                     cparms.maxStockPerFlavour );
        }

        // Create the bottling plant, bank, parent, and WATCard office
        BottlingPlant plant( printer, nameServer, cparms.numVendingMachines, cparms.maxShippedPerFlavour,
                             cparms.maxStockPerFlavour, cparms.timeBetweenShipments );
        Bank bank( cparms.numStudents );
        Parent parent( printer, bank, cparms.numStudents, cparms.parentalDelay );
        WATCardOffice cardOffice( printer, bank, cparms.numCouriers );

        // Create the students
        Student* students[ cparms.numStudents ];
        for ( unsigned int i = 0; i < cparms.numStudents; i += 1 ) {
            students[i] = new Student( printer, nameServer, cardOffice, i, cparms.maxPurchases );
        }
        // Delete each student to end the task
        for ( unsigned int i = 0; i < cparms.numStudents; i += 1 ) {
            delete students[i];
        }

        // Delete each vending machine to end the task
        for ( unsigned int i = 0; i < cparms.numVendingMachines; i += 1 ) {
            delete vendingMachines[i];
        }
    }
}
