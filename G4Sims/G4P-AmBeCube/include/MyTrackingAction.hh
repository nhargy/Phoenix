#ifndef MY_TRACKING_ACTION_HH
#define MY_TRACKING_ACTION_HH

#include "G4UserTrackingAction.hh"

class MyTrackingAction : public G4UserTrackingAction {
    public:
        MyTrackingAction();
        ~MyTrackingAction();

        virtual void PreUserTrackingAction(const G4Track*);

        virtual void PostUserTrackingAction(const G4Track*);
};

#endif
