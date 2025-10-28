#include "MyRunAction.hh"
#include "G4AnalysisManager.hh"
#include <sstream>
#include <string>

MyRunAction::MyRunAction() : outputDirectory("./") {
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

    // Create full path by combining output directory and filename
    G4String filename = outputDirectory;
    if (!filename.empty() && filename.back() != '/') {
        filename += '/';
    }
    filename += "output" + strRunID.str() + ".csv";
    
    man->SetFileName(filename);
    man->OpenFile();

}

void MyRunAction::EndOfRunAction(const G4Run*){

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();

}
