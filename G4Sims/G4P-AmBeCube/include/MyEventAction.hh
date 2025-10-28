#ifndef MY_EVENT_ACTION_HH
#define MY_EVENT_ACTION_HH

#include "G4UserEventAction.hh"

class MyEventAction : public G4UserEventAction {
    public:
        MyEventAction();
        ~MyEventAction();

        virtual void BeginOfEventAction(const G4Event *anEvent);
        virtual void EndOfEventAction(const G4Event *anEvent);
};


#endif
