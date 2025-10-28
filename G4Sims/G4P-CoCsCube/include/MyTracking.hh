#ifndef MY_TRACKING_HH
#define MY_TRACKING_HH

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"
#include "G4Trajectory.hh"
#include "G4Colour.hh"

class MyTrackingAction : public G4UserTrackingAction{
    public:
        MyTrackingAction();
        ~MyTrackingAction();

    virtual void PreUserTrackingAction(const G4Track* track) override;
};

#endif
