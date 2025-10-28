#ifndef MY_STEPPING_ACTION_HH
#define MY_STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class MySteppingAction : public G4UserSteppingAction {
    public:
        MySteppingAction();
        ~MySteppingAction();

        void UserSteppingAction(const G4Step*); 

};

#endif
