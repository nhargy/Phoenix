// =========================================================================
// Project  : G4P-AmBeCube
// File     : main.cc
// Author   : nhargy
// Date     : May 2025
// Brief    : Shielded AmBe simulation in SOREQ
// =========================================================================

#include <iostream>

#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"

#include "MyDetectorConstruction.hh"
#include "MyPhysicsList.hh"
#include "MyActionInitialization.hh"

int main(int argc, char **argv) {

    G4RunManager* runManager = new G4RunManager();
    
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());

    // Get output directory from command line if provided
    G4String outputDir = "./";
    if (argc > 2) {
        outputDir = argv[2];
    }

    // Create action initialization with output directory
    auto actionInit = new MyActionInitialization(outputDir);
    runManager->SetUserInitialization(actionInit);

    // Initialize visualisation only if no macro is passed
    G4UIExecutive* ui = 0;
    if(argc==1){
       ui = new G4UIExecutive(argc, argv);
    }

    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager* uiManager = G4UImanager::GetUIpointer();

    // Set the macro path to include the macros directory relative to the executable
    uiManager->ApplyCommand("/control/macroPath macros/");

    if(ui){
        uiManager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();    
    }
    else{
        G4String command = "/control/execute ";
        G4String filename = argv[1];
        uiManager->ApplyCommand(command + filename);
    }
 
    return 0;
}; 
