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
#include "G4RotationMatrix.hh"
#include "G4UnionSolid.hh"

#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "MyDetectorConstruction.hh"
#include "MySensitiveDetector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction {
    public:
        MyDetectorConstruction();
        ~MyDetectorConstruction();

        virtual G4VPhysicalVolume *Construct(); 

    private:
        // World         := Mother volume  
        // Leg(1,2,3,4)  := Legs on which profile balances
        // Profile       := Main profile
        // Spine         := Longer plane connecting rim
        // SubSpine(1,2) := Two smaller planes connecting rim
        // Cube          := LiF cube
        // Rim           := Aluminium outer rim
        // MidCylinder   := Pb shield middle part
        // EdgeCylinder  := Pb shield edge part       

        // Moderator     := Water container (to be added later)

        virtual void ConstructSDandField();
                    
        /* Messenger variables */
        G4double xCube, yCube, zCube;

        /* Define solid, logical and physical volumes */
        G4Box *solidWorld, *solidFloor;
        G4Box *solidProfile, *solidSpine, *solidSubSpine;
        G4Box *solidLeg;
        G4Box *solidCube;
        G4Tubs *solidRim, *solidCylinder, *solidPlug;
        G4Box *solidWater;
    
        G4LogicalVolume *logicWorld, *logicFloor;
        G4LogicalVolume *logicProfile, *logicSpine, *logicSubSpine;
        G4LogicalVolume *logicLeg;
        G4LogicalVolume *logicCube;
        G4LogicalVolume *logicRim, *logicCylinder, *logicPlug;
        G4LogicalVolume *logicWater;

        G4VPhysicalVolume *physWorld, *physFloor;
        G4VPhysicalVolume *physProfile, *physSpine, *physSubSpine1, *physSubSpine2;
        G4VPhysicalVolume *physLeg1, *physLeg2, *physLeg3, *physLeg4;
        G4VPhysicalVolume *physCube1, *physCube2, *physCube3, *physCube4;
        G4VPhysicalVolume *physRim, *physCylinder, *physPlug1, *physPlug2;
        G4VPhysicalVolume *physWater;

        // Material definitions
        G4Material *Air, *Water, *LiF, *Pb, *Al, *Concrete, *Cadmium, *Paraffin, *Paper;
        void DefineMaterials();

        G4GenericMessenger *fMessengerCube;
};

#endif
