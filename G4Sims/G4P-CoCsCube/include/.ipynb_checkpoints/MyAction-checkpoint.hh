#ifndef MY_ACTION_HH
#define MY_ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "MyGenerator.hh"
#include "MyRun.hh"

class MyActionInitialization : public G4VUserActionInitialization{
    public:
        MyActionInitialization();
        ~MyActionInitialization();

        virtual void Build() const;
};

#endif
