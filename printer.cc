#include <iostream>
using namespace std;
#include "printer.h"


//====================================================
//=================== Data Classes ===================
//====================================================

Printer::Data1::Data1( char s ) : state( s ) { }

void Printer::Data1::print( std::ostream &os, bool moreData ) {
    os << state;
    if ( moreData ) os << '\t';
}


Printer::Data2::Data2( char s, int v1 ) : Data1( s ), value1( v1 ) { }

void Printer::Data2::print( std::ostream &os, bool moreData ) {
    os << state << ' ' << value1;
    if ( moreData ) os << '\t';
}


Printer::Data3::Data3( char s, int v1, int v2 ) : Data2( s, v1 ), value2( v2 ) { }

void Printer::Data3::print( std::ostream &os, bool moreData ) {
    os << state << ' ' << value1 << ' ' << value2;
    if ( moreData ) os << '\t';
}


Printer::DataBuffer::DataBuffer( unsigned int maxSize ) : _maxSize( maxSize ) { }

bool Printer::DataBuffer::hasKey( unsigned int key ) {
    return _buffer.find( key ) != _buffer.end();
}

void Printer::DataBuffer::insert( unsigned int key, Data *data ) {
    _buffer[key] = data;
}

void Printer::DataBuffer::print( std::ostream &os, bool moreData ) {
    for ( unsigned int i = 0; moreData ? i < _maxSize : _buffer.size() > 0; i += 1 ) {
        map<unsigned int, Data*>::iterator it = _buffer.find( i );
      if ( it == _buffer.end() ) { os << '\t'; continue; }
        Data *data = it->second;
        bool moreDataInner = moreData || _buffer.size() > 1;
        data->print( os, moreDataInner );                     // Print the data
        _buffer.erase( it );                                  // Remove the data from the buffer
        delete data;                                          // Delete the data
    }
}


//====================================================
//===================== Printer ======================
//====================================================

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers )
    : _numStudents( numStudents ), _numVendingMachines( numVendingMachines ), _numCouriers( numCouriers )
{
    cout << "Parent\tWATOff\tNames\tTruck\tPlant";
    for ( unsigned int i = 0; i < numStudents; i += 1 ) {
        cout << '\t' << "Stud" << i;
    }
    for ( unsigned int i = 0; i < numVendingMachines; i += 1 ) {
        cout << '\t' << "Mach" << i;
    }
    for ( unsigned int i = 0; i < numCouriers; i += 1 ) {
        cout << '\t' << "Cour" << i;
    }
    cout << endl;

    for ( unsigned int numLeft = 5 + numStudents + numVendingMachines + numCouriers; ; ) {
        cout << "*******";
        numLeft -= 1;
      if ( numLeft == 0 ) break;
        cout << '\t';
    }
    cout << endl;
}

Printer::~Printer() {
    cout << "***********************" << endl;
}

void Printer::print( Kind kind, char state ) {
    if ( state == 'F' ) {
        flush();
        unsigned int numThings = 5 + _numStudents + _numVendingMachines + _numCouriers;
        unsigned int findex = (unsigned int)kind;
        for ( unsigned int i = 0; i < numThings; i += 1 ) {
            if ( i < findex )
                cout << "...\t";
            else if ( i == findex )
                cout << state;
            else
                cout << "\t...";
        }
    } else {
        checkDataBuffer( kind );                              // Flush if the buffer isn't empty
        _buffer[kind] = new Data1( state );                   // Add the data to the buffer
    }
}

void Printer::print( Kind kind, char state, int value1 ) {
    checkDataBuffer( kind );
    _buffer[kind] = new Data2( state, value1 );
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
    checkDataBuffer( kind );
    _buffer[kind] = new Data3( state, value1, value2 );
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
    if ( state == 'F' ) {
        flush();
        unsigned int numThings = 5 + _numStudents + _numVendingMachines + _numCouriers;
        unsigned int findex = 5;
        switch ( kind ) {
          case Printer::Courier: findex += _numVendingMachines;
          case Printer::Vending: findex += _numStudents;
          case Printer::Student: findex += lid;
          default: break;
        }
        for ( unsigned int i = 0; i < numThings; i += 1 ) {
            if ( i < findex )
                cout << "...\t";
            else if ( i == findex )
                cout << state;
            else
                cout << "\t...";
        }
    } else {
        checkDataBuffer( kind, lid );                         // Check if the buffer needs flushed
        DataBuffer *db = (DataBuffer*)_buffer[kind];
        db->insert( kind, new Data1( state ) );               // Add the data to the buffer
    }
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
    checkDataBuffer( kind, lid );
    DataBuffer *db = (DataBuffer*)_buffer[kind];
    db->insert( kind, new Data2( state, value1 ) );
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    checkDataBuffer( kind, lid );
    DataBuffer *db = (DataBuffer*)_buffer[kind];
    db->insert( kind, new Data3( state, value1, value2 ) );
}

unsigned int Printer::getSizeOfKind( Kind kind ) {
    switch ( kind ) {
      case Printer::Student: return _numStudents;
      case Printer::Vending: return _numVendingMachines;
      case Printer::Courier: return _numCouriers;
      default: return 1;
    }
}

void Printer::checkDataBuffer( Kind kind ) {
    if ( _buffer.find( kind ) != _buffer.end() ) flush();     // Flush if there is data for the Kind in the buffer
}

void Printer::checkDataBuffer( Kind kind, unsigned int lid ) {
    map<unsigned int, Data*>::iterator it = _buffer.find( kind );
    if ( it == _buffer.end() ) {                              // If the buffer for the kind doesn't exist, create it
        unsigned int kindSize = getSizeOfKind( kind );
        _buffer[kind] = new DataBuffer( kindSize );
    } else if ( ((DataBuffer*)it->second)->hasKey( lid ) ) {  // If there is data for the specific item, flush
        flush();
        // Flush deletes the DataBuffer so it needs to be created again
        unsigned int kindSize = getSizeOfKind( kind );
        _buffer[kind] = new DataBuffer( kindSize );
    }
}

void Printer::flush() {
    if ( _buffer.size() == 0 ) return;

    for ( unsigned int i = 0; _buffer.size() > 0; i += 1 ) {
        map<unsigned int, Data*>::iterator it = _buffer.find( i );
    if ( it == _buffer.end() ) {
        unsigned int kindSize = getSizeOfKind( (Kind) i );
        for ( unsigned int i = 0; i < kindSize; i += 1 ) {
            cout << '\t';
        }
        continue;
    }
        Data *data = it->second;
        bool moreData = _buffer.size() > 1;
        data->print( cout, moreData );                   // Print the data
        _buffer.erase( it );                             // Remove the data from the buffer
        delete data;                                     // Delete the data
    }
    cout << endl;
}
