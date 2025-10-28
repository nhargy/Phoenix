#include "G4TrackingManager.hh"
#include "G4VTrajectory.hh"
#include "G4Trajectory.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "MyTracking.hh"

MyTrackingAction::MyTrackingAction(){
};

MyTrackingAction::~MyTrackingAction(){
};

void MyTrackingAction::PreUserTrackingAction(const G4Track* track){
    G4int pdg = track->GetDefinition()->GetPDGEncoding();

    // Kill electron neutrinos
    if (pdg == 12 || pdg == -12) {
        G4Track* mutableTrack = const_cast<G4Track*>(track);
        mutableTrack->SetTrackStatus(fStopAndKill);
        return;
    }
};
