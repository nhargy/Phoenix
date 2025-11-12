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

#include "MySensitiveDetector.hh"

using std::vector;
using std::map;

class MyDetectorConstruction : public G4VUserDetectorConstruction {
    public:
    MyDetectorConstruction(const G4String& outputPath = "./");
        ~MyDetectorConstruction();

        virtual G4VPhysicalVolume *Construct(); 

    private:

        static map<G4String, G4double> m_hGeoParams;
        G4String fOutputDirectory;

        // Save geometry parameters to CSV (written to fOutputDirectory)
        void SaveGeoParamsToCSV() const;

        G4bool usePLYWheel  = true;  // default
        G4bool usePLYBlocks = true;  // default

        void DefineGeoParams();
        void DefineMaterials();

        void ConstructLab();
        void ConstructFrame();
        void ConstructWheel();
        void ConstructPLYWheel();
        void ConstructSourceShield();
        void ConstructCrystals();
        void ConstructPLYBlocks();

        virtual void ConstructSDandField();
                    
        /* Messenger variables */
        G4GenericMessenger *fMessengerCube;

        /* Lab */
        G4Box*             solid_Lab;
        G4LogicalVolume*   logic_Lab;
        G4VPhysicalVolume* phys_Lab;

        G4Box*             solid_Floor;
        G4LogicalVolume*   logic_Floor;
        G4VPhysicalVolume* phys_Floor;

        /* Frame */
        G4LogicalVolume*   logic_Frame;
        G4VPhysicalVolume* phys_Frame;

        /* Wheel */
        G4LogicalVolume*   logic_Wheel;
        G4VPhysicalVolume* phys_Wheel;

        /* PLYWheel */
        G4LogicalVolume*   logic_PLYWheel;
        G4VPhysicalVolume* phys_PLYWheel;

        /* SourceShield */
        G4LogicalVolume*   logic_SourceShield;
        G4VPhysicalVolume* phys_SourceShield;

        /* PLYBlocks */
        G4LogicalVolume*   logic_PLYBlock;
        G4VPhysicalVolume* phys_PLYBlock0;
        G4VPhysicalVolume* phys_PLYBlock1;

        /* Crystals */
        G4LogicalVolume*   logic_Crystal;
        G4VPhysicalVolume* phys_Crystal0;
        G4VPhysicalVolume* phys_Crystal1;
        G4VPhysicalVolume* phys_Crystal2;
        G4VPhysicalVolume* phys_Crystal3;

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