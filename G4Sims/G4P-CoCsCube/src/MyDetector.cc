#include "MyDetector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) 
    : G4VSensitiveDetector(name){
}

MySensitiveDetector::~MySensitiveDetector(){
}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, 
                                        G4TouchableHistory *R0hist){ 

    G4Track *track = aStep->GetTrack();
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    const G4VProcess *preProc = preStepPoint->GetProcessDefinedStep(); 
    G4String preProcName = "NA";
    if (preProc) preProcName = preProc->GetProcessName();

    const G4VProcess *postProc = postStepPoint->GetProcessDefinedStep(); 
    G4String postProcName = "NA";
    if (postProc) postProcName = postProc->GetProcessName();

    G4bool isEntry = (preStepPoint->GetStepStatus()==fGeomBoundary);

    G4int copyNo = preStepPoint->GetTouchableHandle()->GetCopyNumber();
    G4int trackID = track->GetTrackID();
    G4int parentID = track->GetParentID();

    // Kill track at interface with LiF cube if thermal neutron
    // track->SetTrackStatus(fStopAndKill);

    G4ThreeVector prePos = preStepPoint->GetPosition();
    G4ThreeVector postPos = postStepPoint->GetPosition();
    //G4ThreeVector pos = 0.5 * (prePos + postPos);

    G4double edep = aStep->GetTotalEnergyDeposit();
    //if(edep==0) return false;

    G4double kinetic = preStepPoint->GetKineticEnergy();

    G4VPhysicalVolume* volume = preStepPoint->GetTouchableHandle()->GetVolume();
    G4String volName = volume->GetName();

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int pdgID = track->GetDefinition()->GetPDGEncoding();

    //if((pdgID==2112) & (isEntry==1) & (kinetic < 0.000001)){
    //    track->SetTrackStatus(fStopAndKill);
    //    edep = 4.78;
    //    postProcName = "nCusCap";
    //}

    man->FillNtupleIColumn(0, evt);
    man->FillNtupleIColumn(1, isEntry);
    man->FillNtupleSColumn(2, preProcName);
    man->FillNtupleSColumn(3, postProcName);
    man->FillNtupleIColumn(4, trackID);
    man->FillNtupleIColumn(5, parentID);
    man->FillNtupleIColumn(6, pdgID);
    man->FillNtupleDColumn(7, kinetic);
    man->FillNtupleDColumn(8, edep);
    man->FillNtupleDColumn(9, prePos[0]);
    man->FillNtupleDColumn(10, prePos[1]);
    man->FillNtupleDColumn(11, prePos[2]);
    man->FillNtupleDColumn(12, postPos[0]);
    man->FillNtupleDColumn(13, postPos[1]);
    man->FillNtupleDColumn(14, postPos[2]);
    man->FillNtupleIColumn(15, copyNo);
    man->AddNtupleRow(0);

    return true;
}
