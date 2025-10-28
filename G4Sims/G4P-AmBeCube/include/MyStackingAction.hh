#ifndef MY_STACKING_ACTION_HH
#define MY_STACKING_ACTION_HH

#include "G4UserStackingAction.hh"

class MyStackingAction : public G4UserStackingAction {
    public:
        MyStackingAction();
        ~MyStackingAction();

        virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *aTrack);

        virtual void NewStage();

        virtual void PrepareNewEvent(); 
};

#endif
