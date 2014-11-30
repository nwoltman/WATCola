#include <cstdlib>            // atoll
#include <iostream>
#include <fstream>
using namespace std;
#include "config.h"
#include "mprng.h"


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
    ConfigParms configParms;
    unsigned int seed = getpid();

    switch ( argc ) {
      case 3:
        if ( ! isPosInt( argv[2] ) ) printUsageAndExit( argv[0] );
        seed = atoll( argv[2] );
      case 2:
        processConfigFile( argv[1], configParms );       // Parse the specified config file
        break;
      case 1:
        processConfigFile( "soda.config", configParms ); // Parse the default config file
        break;
      default:
        printUsageAndExit( argv[0] );
    }

    g_mprng.seed( seed );                                // Seed the MPRNG

}
