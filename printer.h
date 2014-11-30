#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <map>


_Monitor Printer {
    class Data {                      // Abstract Data class
      public:
        virtual ~Data() { }
        virtual void print( std::ostream &os, bool moreData = false ) = 0;
    };

    class Data1 : public Data {       // Data that only has a state value
      protected:
        char state;
      public:
        Data1( char s );
        void print( std::ostream &os, bool moreData = false);
    };

    class Data2 : public Data1 {      // Data that has a state and an integer value
      protected:
        int value1;
      public:
        Data2( char s, int v1 );
        void print( std::ostream &os, bool moreData = false );
    };

    class Data3 : public Data2 {      // Data that has a state and 2 integer values
      protected:
        int value2;
      public:
        Data3( char s, int v1, int v2 );
        void print( std::ostream &os, bool moreData = false );
    };

    class DataBuffer : public Data {  // Data for Students, VendingMachines, and Couriers
        unsigned int _maxSize;
        std::map<unsigned int, Data*> _buffer;
      public:
        DataBuffer( unsigned int maxSize );
        bool hasKey( unsigned int key );
        void insert( unsigned int key, Data *data );
        void print( std::ostream &os, bool moreData = false );
    };

    unsigned int _numStudents;
    unsigned int _numVendingMachines;
    unsigned int _numCouriers;
    std::map<unsigned int, Data*> _buffer;
  public:
    enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
  private:
    unsigned int getSizeOfKind( Kind kind );  // Determines the number of columns of output the Kind takes up
    void checkDataBuffer( Kind kind );        // Checks if data will be overwritten and flushes
    void checkDataBuffer( Kind kind, unsigned int lid ); // Same as above but also creates a new DataBuffer if needed
    void flush();                             // Flushes the stored data to stdout
};

#endif
