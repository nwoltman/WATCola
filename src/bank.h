#ifndef BANK_H
#define BANK_H


_Monitor Bank {
    unsigned int _numStudents;
    unsigned int *_accounts;
  public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif
