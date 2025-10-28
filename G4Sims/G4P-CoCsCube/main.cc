// =========================================================================
// Project: G4P-CoCsCube
// File   : main.cc
// Author : nhargy
// Date   : April 2025
// Brief  : Cobalt-60 and Caesium-137 ER simulations
// =========================================================================

#include <iostream>

#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"

#include "MyAction.hh"
#include "MyConstruction.hh"
#include "MyPhysics.hh"

#include <fstream>

int main(int argc, char **argv){
   
    G4RunManager* runManager = new G4RunManager();

    runManager->SetUserInitialization(new MyDetectorConstruction()); 
    runManager->SetUserInitialization(new MyPhysicsList()); 

    // Get output directory from command line if provided
    G4String outputDir = "./";
    if (argc > 2) {
        outputDir = argv[2];
    }

    auto actionInit = new MyActionInitialization();
    auto runAction = dynamic_cast<MyRunAction*>(actionInit->GetUserRunAction());
    if (runAction) {
        runAction->SetOutputDirectory(outputDir);
    }
    
    runManager->SetUserInitialization(actionInit);

    //runManager->Initialize();

    // Initialize visualisation only if no macro is passed
    G4UIExecutive* ui = 0;
    if(argc==1){
       ui = new G4UIExecutive(argc, argv);
    }

    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager* uiManager = G4UImanager::GetUIpointer();

    if(ui){
        uiManager->ApplyCommand("/control/execute macros/vis.mac");
        ui->SessionStart();    
    }
    else{
        G4String command = "/control/execute ";
        G4String filename = argv[1];

        // If the file as given doesn't exist, try the local macros/ directory
        std::ifstream f(filename.c_str());
        if (!f.good()) {
            G4String alt = "macros/" + filename;
            std::ifstream f2(alt.c_str());
            if (f2.good()) {
                filename = alt;
            }
            // else leave filename as given; UI will report error if not found
        }

        uiManager->ApplyCommand(command + filename);
    }
    
    return 0;
} 
