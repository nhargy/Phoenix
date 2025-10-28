#ifndef MY_CONSTRUCTION_HH
#define MY_CONSTRUCTION_HH

#include "G4Box.hh"
#include "G4GenericMessenger.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"

#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "MyDetector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction{
    public:
        MyDetectorConstruction();        
        ~MyDetectorConstruction();

        virtual G4VPhysicalVolume* Construct();

    private:
        // World  := Mother volume
        // Holder := Circular plastic crystal holder
        // Block  := Lead blocks making up the shielding
        // Floor  := Concrete floot
        // Cube   := The LiF cube detector

        // Messenger variables
        G4double CubeDistance, CubeSide;

        G4LogicalVolume *logicCube;
        virtual void ConstructSDandField();

        G4Box *solidWorld, *solidFloor, *solidCube;
        G4Box *solidBlockBottom; 
        G4Box *solidBlockSide1, *solidBlockSide2, *solidBlockSide3, *solidBlockSide4;
        G4Tubs *solidCylinder, *solidTube;
        G4Tubs *solidHolder;

        G4LogicalVolume *logicWorld, *logicFloor;
        G4LogicalVolume *logicBlockBottom; 
        G4LogicalVolume *logicBlockSide1, *logicBlockSide2, *logicBlockSide3, *logicBlockSide4;
        G4LogicalVolume *logicCylinder, *logicTube;
        G4LogicalVolume *logicHolder;

        G4VPhysicalVolume *physWorld, *physFloor, *physCube;
        G4VPhysicalVolume *physBlockBottom;
        G4VPhysicalVolume *physBlockSide1, *physBlockSide2, *physBlockSide3, *physBlockSide4;
        G4VPhysicalVolume *physCylinder, *physTube;
        G4VPhysicalVolume *physHolder; 

        // Material definitions
        G4Material *Air, *LiF, *Pb, *PLA, *Concrete;
        void DefineMaterials();

        G4GenericMessenger *fMessengerCube;
};

#endif
