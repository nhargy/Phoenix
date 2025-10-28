#include "MyRun.hh"
#include "G4AnalysisManager.hh"
#include <sstream>
#include <cstdio> // for std::rename

MyRunAction::MyRunAction() : fOutputDirectory("./") {

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleIColumn("fEntry");   
    man->CreateNtupleSColumn("fPreProc");
    man->CreateNtupleSColumn("fPostProc");
    man->CreateNtupleIColumn("fTrackID");
    man->CreateNtupleIColumn("fParentID");
    man->CreateNtupleIColumn("fPDG");
    man->CreateNtupleDColumn("fKinetic");
    man->CreateNtupleDColumn("fEdep");
    man->CreateNtupleDColumn("fX1");
    man->CreateNtupleDColumn("fY1");
    man->CreateNtupleDColumn("fZ1");
    man->CreateNtupleDColumn("fX2");
    man->CreateNtupleDColumn("fY2");
    man->CreateNtupleDColumn("fZ2");
    man->CreateNtupleIColumn("Copy");
    man->FinishNtuple(0);

}

MyRunAction::~MyRunAction(){
}

void MyRunAction::BeginOfRunAction(const G4Run* run){

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    G4String filename = fOutputDirectory;
    if (filename.back() != '/') filename += "/";
    filename += "run_" + strRunID.str() + ".csv";
    
    man->SetFileName(filename);
    man->OpenFile();

}

void MyRunAction::EndOfRunAction(const G4Run* run){

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();

    // Geant4 CSV backend creates files named like
    // <basename>_nt_Hits.csv (nt_ + ntuple name). Rename that to
    // <basename>.csv if present so the file is simply run_<id>.csv.
    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;

    G4String base = fOutputDirectory;
    if (base.size() == 0) base = "./";
    if (base.back() != '/') base += "/";

    G4String produced = base + "run_" + strRunID.str() + "_nt_Hits.csv";
    G4String desired  = base + "run_" + strRunID.str() + ".csv";

    // Try to rename; if it fails, print a message but continue.
    if (std::rename(produced.c_str(), desired.c_str()) != 0) {
        G4cout << "[MyRunAction] Could not rename '" << produced << "' to '" << desired << "' - file may not exist or permission denied." << G4endl;
    } else {
        G4cout << "[MyRunAction] Renamed output file to '" << desired << "'" << G4endl;
    }

}
