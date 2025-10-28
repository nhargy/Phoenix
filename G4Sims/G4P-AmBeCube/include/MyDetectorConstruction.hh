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
#include "G4SubtractionSolid.hh"

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

        static map<G4String, G4double> m_hGeoParams;

        void DefineGeoParams();
        void DefineMaterials();
        void ConstructLab();
        void ConstructFrame();
        void ConstructWheel();
        void ConstructSourceShield();
        void ConstructCrystals();
        void ConstructModerators();

        virtual void ConstructSDandField();
                    
        /* Messenger variables */
        G4GenericMessenger *fMessengerCube;

        /* Declarations: Solid Volumes*/
        G4Box *solidWorld,
              *solidFloor,
              *solidCube;

        G4Tubs *solidSourceShield;

        G4VSolid *solidFrame,
                 *solidWheel;

        /* Declarations: Logical Volumes*/
        G4LogicalVolume *logicWorld, 
                        *logicFloor,
                        *logicCube,
                        *logicSourceShield,
                        *logicFrame,
                        *logicWheel;

        /* Declarations: Physical Volumes*/
        G4VPhysicalVolume *physWorld, 
                          *physFloor,
                          *physCube,
                          *physSourceShield,
                          *physFrame,
                          *physWheel;

        // Material definitions
        G4Element *C,
                  *H,
                  *O; 

        G4Material *Air, 
                   *Water, 
                   *LiF, 
                   *Pb, 
                   *Al, 
                   *Concrete, 
                   *Polyethylene, 
                   *Paper;

};

#endif