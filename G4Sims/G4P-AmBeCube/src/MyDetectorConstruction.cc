#include "MyDetectorConstruction.hh"

map<G4String, G4double> MyDetectorConstruction::m_hGeoParams;

MyDetectorConstruction::MyDetectorConstruction() {
};

MyDetectorConstruction::~MyDetectorConstruction() {
};


G4VPhysicalVolume *MyDetectorConstruction::Construct() {

    DefineGeoParams();
    DefineMaterials();
    ConstructLab();
    ConstructFrame();

    if (usePLYWheel) {
        ConstructPLYWheel();
    } else {
        ConstructWheel();
    }

    if (usePLYBlocks) {
        ConstructPLYBlocks();
    }
    
    ConstructSourceShield();
    ConstructCrystals();

    return physWorld;
}

void MyDetectorConstruction::DefineGeoParams() {

    // ======= Lab ======= //
    m_hGeoParams["xWorld"] = 100. *cm;
    m_hGeoParams["yWorld"] = 100. *cm;
    m_hGeoParams["zWorld"] = 200. *cm;

    m_hGeoParams["zFloor"] =  20. *cm;

    // ======= Profiles ======= //
    m_hGeoParams["xProfile"]      =  4. *cm;
    m_hGeoParams["yProfile"]      =  4. *cm;
    m_hGeoParams["zProfileLong"]  = 99. *cm;
    m_hGeoParams["zProfileShort"] = 30. *cm;

    // ======= Wheel ======= //
    m_hGeoParams["oRadWheel"]  = 15.   *cm;
    m_hGeoParams["iRadWheel"]  = 12.   *cm;
    m_hGeoParams["zWheel"]     =  0.05 *cm;

    m_hGeoParams["wSpoke"] = 2. *cm;

    m_hGeoParams["zPaper"] = 0.01 *cm;

    // ======= PLY Wheel ======= //
    m_hGeoParams["oRadPLYWheel"]  = 15.  *cm;
    m_hGeoParams["iRadPLYWheel"]  =  0.  *cm;
    m_hGeoParams["zPLYWheel"]     =  1.5 *cm;

    // ======= PLY Blocks ======= //
    m_hGeoParams["xPLYBlock"] =  5. *cm;
    m_hGeoParams["yPLYBlock"] = 20. *cm;
    m_hGeoParams["zPLYBlock"] = 10. *cm;

    // ======= Source Shield ======= //
    m_hGeoParams["oRadSourceShield"] = 3.35 *cm;
    m_hGeoParams["iRadSourceShield"] = 2.85 *cm;
    m_hGeoParams["zSourceShield"]    = 10.2 *cm;
    m_hGeoParams["zPlug"]            = 1.7  *cm;

    // ======= Crystals ======= //
    m_hGeoParams["xCrystal"] = 1. *cm;
    m_hGeoParams["yCrystal"] = 1. *cm;
    m_hGeoParams["zCrystal"] = 1. *cm;

    m_hGeoParams["RadCrystalSource"] = 10. *cm;
};

void MyDetectorConstruction::DefineMaterials() {

    G4NistManager *nist = G4NistManager::Instance();

    Air          = nist->FindOrBuildMaterial("G4_AIR");
    LiF          = nist->FindOrBuildMaterial("G4_LITHIUM_FLUORIDE");
    Al           = nist->FindOrBuildMaterial("G4_Al");
    Water        = nist->FindOrBuildMaterial("G4_WATER");
    Pb           = nist->FindOrBuildMaterial("G4_Pb");
    Concrete     = nist->FindOrBuildMaterial("G4_CONCRETE");
    Polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

    
    // Build paper: wood-based paper roughly C₆H₁₀O₅ (cellulose)
    C = nist->FindOrBuildElement("C");
    H = nist->FindOrBuildElement("H");
    O = nist->FindOrBuildElement("O");

    Paper = new G4Material("Paper", 0.8*g/cm3, 3);
    Paper->AddElement(C, 6);
    Paper->AddElement(H,10);
    Paper->AddElement(O, 5);
};

void MyDetectorConstruction::ConstructLab() {
    
    // **** Build World **** //
    G4double xWorld = m_hGeoParams["xWorld"];
    G4double yWorld = m_hGeoParams["yWorld"];
    G4double zWorld = m_hGeoParams["zWorld"];

    G4double xPosWorld = 0.*cm;
    G4double yPosWorld = 0.*cm;
    G4double zPosWorld = 0.*cm;

    auto vecWorld = G4ThreeVector(xPosWorld, yPosWorld, zPosWorld);

    solidWorld = new G4Box("solidWorld",
                           xWorld/2,
                           yWorld/2,
                           zWorld/2
                        );

    logicWorld = new G4LogicalVolume(solidWorld,
                                    Air,
                                    "logicWorld"
                                );

    physWorld  = new G4PVPlacement(0,
                                   vecWorld,
                                   logicWorld,
                                   "physWorld",
                                   0,
                                   false,
                                   0,
                                   true
                                );

    // **** Build Floor **** //
    G4double xFloor = xWorld;
    G4double yFloor = yWorld;
    G4double zFloor = m_hGeoParams["zFloor"];

    G4double xPosFloor = 0.*cm;
    G4double yPosFloor = 0.*cm;
    G4double zPosFloor = (-zWorld/2 + zFloor/2);
    
    auto vecFloor = G4ThreeVector(xPosFloor, yPosFloor, zPosFloor);    

    solidFloor = new G4Box("solidFloor",
                           xFloor/2,
                           yFloor/2,
                           zFloor/2
                        );

    logicFloor = new G4LogicalVolume(solidFloor,
                                    Concrete,
                                    "logicFloor"
                                );

    physFloor  = new G4PVPlacement(0,
                                   vecFloor,
                                   logicFloor,
                                   "physFloor",
                                   logicWorld,
                                   false,
                                   0,
                                   true
                                ); 
    
    /* Colours & Styling */
    auto colFloor = G4Colour(0.5, 0.5, 0.5);
    G4VisAttributes *visAttributesFloor = new G4VisAttributes(colFloor);
    visAttributesFloor->SetVisibility(true);
    visAttributesFloor->SetForceSolid(true);
    logicFloor->SetVisAttributes(visAttributesFloor);
};

void MyDetectorConstruction::ConstructFrame() {

    G4double zWorld = m_hGeoParams["zWorld"];
    G4double zFloor = m_hGeoParams["zFloor"];

    G4double xProfile     = m_hGeoParams["xProfile"];
    G4double yProfile     = m_hGeoParams["yProfile"];
    G4double zProfileLong = m_hGeoParams["zProfileLong"];
    G4double zProfileShort= m_hGeoParams["zProfileShort"];


    G4Box *solidProfileLong = new G4Box("solidProfileLong",
                                       m_hGeoParams["xProfile"]/2,
                                       m_hGeoParams["yProfile"]/2,
                                       m_hGeoParams["zProfileLong"]/2
                                    );

    G4Box *solidProfileShort = new G4Box("solidProfileShort",
                                       m_hGeoParams["xProfile"]/2,
                                       m_hGeoParams["yProfile"]/2,
                                       m_hGeoParams["zProfileShort"]/2
                                    );

    // Add Leg1
    G4double xTemp1 = xProfile/2 + zProfileShort/2; 
    G4double yTemp1 = 0. *cm;
    G4double zTemp1 = -zProfileLong/2 + xProfile/2;
    auto vecTemp1 = G4ThreeVector(xTemp1, yTemp1, zTemp1);
    G4RotationMatrix* rotTemp1 = new G4RotationMatrix;
    rotTemp1->rotateY(90*deg);

    G4VSolid *solidTemp1 = new G4UnionSolid("Temp1",
                                            solidProfileLong,
                                            solidProfileShort,
                                            rotTemp1,
                                            vecTemp1
                                        );

    // Add Leg2
    G4double xTemp2 = -(xProfile/2 + zProfileShort/2); 
    G4double yTemp2 = 0. *cm;
    G4double zTemp2 = -zProfileLong/2 + xProfile/2;
    auto vecTemp2 = G4ThreeVector(xTemp2, yTemp2, zTemp2);
    G4RotationMatrix* rotTemp2 = new G4RotationMatrix;
    rotTemp2->rotateY(90*deg);

    G4VSolid *solidTemp2 = new G4UnionSolid("Temp2",
                                            solidTemp1,
                                            solidProfileShort,
                                            rotTemp2,
                                            vecTemp2
                                        );

    // Add Leg3
    G4double xTemp3 = 0. *cm; 
    G4double yTemp3 = xProfile/2 + zProfileShort/2;
    G4double zTemp3 = -zProfileLong/2 + xProfile/2;
    auto vecTemp3 = G4ThreeVector(xTemp3, yTemp3, zTemp3);
    G4RotationMatrix* rotTemp3 = new G4RotationMatrix;
    rotTemp3->rotateX(90*deg);

    G4VSolid *solidTemp3 = new G4UnionSolid("Temp3",
                                            solidTemp2,
                                            solidProfileShort,
                                            rotTemp3,
                                            vecTemp3
                                        );

    // Add Leg4
    G4double xTemp4 = 0. *cm; 
    G4double yTemp4 = -(xProfile/2 + zProfileShort/2);
    G4double zTemp4 = -zProfileLong/2 + xProfile/2;
    auto vecTemp4 = G4ThreeVector(xTemp4, yTemp4, zTemp4);
    G4RotationMatrix* rotTemp4 = new G4RotationMatrix;
    rotTemp4->rotateX(90*deg);

    solidFrame = new G4UnionSolid("solidFrame",
                                  solidTemp3,
                                  solidProfileShort,
                                  rotTemp4,
                                  vecTemp4
                                );

    logicFrame = new G4LogicalVolume(solidFrame,
                                     Al,
                                     "logicFrame"
                                    );

    G4double zFramePos = -zWorld/2 + zFloor + zProfileLong/2;
    auto vecFrame = G4ThreeVector(0., 0., zFramePos);
    G4PVPlacement *physFrame = new G4PVPlacement(0,
                                                 vecFrame,
                                                 logicFrame,
                                                 "physFrame",
                                                 logicWorld,
                                                 false,
                                                 0,
                                                 true
                                                 );

    /* Colours & Styling */
    auto colFrame = G4Colour(0., 0.8, 1.);
    G4VisAttributes *visAttributesFrame = new G4VisAttributes(colFrame);
    visAttributesFrame->SetVisibility(true);
    visAttributesFrame->SetForceSolid(true);
    logicFrame->SetVisAttributes(visAttributesFrame);
};

void MyDetectorConstruction::ConstructWheel() {

    G4double oRadWheel = m_hGeoParams["oRadWheel"];
    G4double iRadWheel = m_hGeoParams["iRadWheel"];
    G4double zWheel    = m_hGeoParams["zWheel"];

    G4double wSpoke    = m_hGeoParams["wSpoke"];
    G4double zPaper    = m_hGeoParams["zPaper"];

    G4double zWorld = m_hGeoParams["zWorld"];
    G4double zFloor = m_hGeoParams["zFloor"];
    G4double zProfileLong = m_hGeoParams["zProfileLong"];



    G4Tubs *solidRim = new G4Tubs("solidRim",
                                  iRadWheel,
                                  oRadWheel,
                                  zWheel/2,
                                  0,
                                  360 *deg
                            );

    G4Tubs *solidPaper = new G4Tubs("solidPaper",
                                    0.,
                                    oRadWheel,
                                    zPaper/2,
                                    0.,
                                    360* deg
                                );

    G4Box *solidSpoke = new G4Box("solidSpoke",
                                   iRadWheel + 0.1*cm,
                                   wSpoke/2,
                                   zWheel/2
                                );

    G4UnionSolid *solidTemp1 = new G4UnionSolid("Temp1",
                                solidRim,
                                solidSpoke,
                                0,
                                G4ThreeVector(0,0,0)
                                );

    G4RotationMatrix* rotZ = new G4RotationMatrix;
    rotZ->rotateZ(90*deg);

    G4UnionSolid *solidTemp2 = new G4UnionSolid("Temp2",
                                solidTemp1,
                                solidSpoke,
                                rotZ, 
                                G4ThreeVector(0,0,0)
                                );

    G4UnionSolid *solidWheel = new G4UnionSolid("solidWheel",
                                solidTemp2,
                                solidPaper,
                                nullptr,
                                G4ThreeVector(0., 0., zWheel/2+zPaper/2)
                                );

    logicWheel = new G4LogicalVolume(solidWheel,
                                     Al,
                                     "logicWheel"
                                    );

    G4double zWheelPos = -zWorld/2
                        + zFloor
                        + zProfileLong
                        + zWheel/2
                        + zPaper/2;
    auto vecWheel = G4ThreeVector(0., 0., zWheelPos);        

    G4RotationMatrix* rotZ45 = new G4RotationMatrix;
    rotZ45->rotateZ(45*deg);
    physWheel = new G4PVPlacement(rotZ45,
                                  vecWheel,
                                  logicWheel,
                                  "logicWheel",
                                  logicWorld,
                                  false,
                                  0,
                                  true
                                ); 

    /* Colours & Styling */
    auto colWheel = G4Colour(0.7, 0.7, 0.7);
    G4VisAttributes *visAttributesWheel = new G4VisAttributes(colWheel);
    visAttributesWheel->SetVisibility(true);
    visAttributesWheel->SetForceSolid(true);
    logicWheel->SetVisAttributes(visAttributesWheel);

}

void MyDetectorConstruction::ConstructPLYWheel() {

    G4double oRadPLYWheel = m_hGeoParams["oRadPLYWheel"];
    G4double iRadPLYWheel = m_hGeoParams["iRadPLYWheel"];
    G4double zPLYWheel    = m_hGeoParams["zPLYWheel"];

    G4double zProfileLong = m_hGeoParams["zProfileLong"];
    G4double zFloor       = m_hGeoParams["zFloor"];
    G4double zWorld       = m_hGeoParams["zWorld"];

    solidPLYWheel = new G4Tubs("solidPLYWheel",
                                iRadPLYWheel,
                                oRadPLYWheel,
                                zPLYWheel/2,
                                0,
                                360*deg
                               );

    logicPLYWheel = new G4LogicalVolume(solidPLYWheel,
                                         Polyethylene,
                                         "logicPLYWheel"
                                        );

    G4double zPLYWheelPos = -zWorld/2 + zFloor + zProfileLong + zPLYWheel/2;
    auto vecPLYWheel = G4ThreeVector(0., 0., zPLYWheelPos);
    
    physPLYWheel = new G4PVPlacement(0,
                                      vecPLYWheel,
                                      logicPLYWheel,
                                      "physPLYWheel",
                                      logicWorld,
                                      false,
                                      0,
                                      true
                                     );

    /* Colours & Styling */
    auto colPLYWheel = G4Colour(1., 0.9, 0.9);
    G4VisAttributes *visAttributesPLYWheel = new G4VisAttributes(colPLYWheel);
    visAttributesPLYWheel->SetVisibility(true);
    visAttributesPLYWheel->SetForceSolid(true);
    logicPLYWheel->SetVisAttributes(visAttributesPLYWheel);
}

void MyDetectorConstruction::ConstructSourceShield() {

    G4double oRadSourceShield = m_hGeoParams["oRadSourceShield"];
    G4double iRadSourceShield = m_hGeoParams["iRadSourceShield"];
    G4double zSourceShield    = m_hGeoParams["zSourceShield"];
    G4double zPlug            = m_hGeoParams["zPlug"];

    G4double zPLYWheel    = m_hGeoParams["zPLYWheel"];
    G4double zWheel       = m_hGeoParams["zWheel"];
    G4double zPaper       = m_hGeoParams["zPaper"];
    G4double zProfileLong = m_hGeoParams["zProfileLong"];
    G4double zFloor       = m_hGeoParams["zFloor"];
    G4double zWorld       = m_hGeoParams["zWorld"];

    G4double zWheelEff;
    if (usePLYWheel) {
        zWheelEff = zPLYWheel;
    } else {
        zWheelEff = zWheel + zPaper;
    }


    G4Tubs *solidCylinder = new G4Tubs("solidCylinder",
                                    iRadSourceShield,
                                    oRadSourceShield,
                                    zSourceShield/2,
                                    0,
                                    360*deg
                                   );

    G4Tubs *solidPlug = new G4Tubs("solidPlug",
                                    0.,
                                    iRadSourceShield + 0.1*cm,
                                    zPlug/2,
                                    0,
                                    360*deg
                                   );

    auto vecTemp1 = G4ThreeVector(0., 0., (zSourceShield/2 - zPlug/2));
    G4UnionSolid *solidTemp1 = new G4UnionSolid("Temp1",
                                                    solidCylinder,
                                                    solidPlug,
                                                    0,
                                                    vecTemp1
                                                );

    auto vecTemp2 = G4ThreeVector(0., 0., (-zSourceShield/2 + zPlug/2));
    G4UnionSolid *solidSourceShield = new G4UnionSolid("Temp2",
                                                    solidTemp1,
                                                    solidPlug,
                                                    0,
                                                    vecTemp2
                                                ); 
                                    
    logicSourceShield = new G4LogicalVolume(solidSourceShield,
                                            Pb,
                                            "logicSourceShield"
                                        ); 

    G4double zSourceShieldPos = -zWorld/2 
                                + zFloor 
                                + zProfileLong 
                                + zWheelEff 
                                + zSourceShield/2;
    auto vecSourceShield = G4ThreeVector(0., 
                                         0.,
                                         zSourceShieldPos);
    physSourceShield = new G4PVPlacement(0,
                                   vecSourceShield,
                                   logicSourceShield,
                                   "physSourceShield",
                                   logicWorld,
                                   false,
                                   0,
                                   true
                                );

    /* Colours & Styling */
    auto colSourceShield = G4Colour(1., 0., 0.);
    G4VisAttributes *visAttributesSourceShield = new G4VisAttributes(
                                                            colSourceShield);
    visAttributesSourceShield->SetVisibility(true);
    visAttributesSourceShield->SetForceSolid(true);
    logicSourceShield->SetVisAttributes(visAttributesSourceShield);

}

void MyDetectorConstruction::ConstructPLYBlocks(){

    G4double xPLYBlock = m_hGeoParams["xPLYBlock"];
    G4double yPLYBlock = m_hGeoParams["yPLYBlock"];
    G4double zPLYBlock = m_hGeoParams["zPLYBlock"];

    G4double zPLYWheel       = m_hGeoParams["zPLYWheel"];
    G4double zProfileLong    = m_hGeoParams["zProfileLong"];
    G4double zFloor          = m_hGeoParams["zFloor"];
    G4double zWorld          = m_hGeoParams["zWorld"];

    G4double oRadSourceShield = m_hGeoParams["oRadSourceShield"];

    solidPLYBlock = new G4Box("solidPLYBlock",
                              xPLYBlock/2,
                              yPLYBlock/2,
                              zPLYBlock/2
                             );

    logicPLYBlock = new G4LogicalVolume(solidPLYBlock,
                                        Polyethylene,
                                        "logicPLYBlock"
                                       );

    G4double zPLYBlockPos = -zWorld/2 
                            + zFloor 
                            + zProfileLong 
                            + zPLYWheel 
                            + zPLYBlock/2;

    // Place Block 1
    G4double xPosPLYBlock1 = oRadSourceShield + xPLYBlock/2 + 0.1*cm;
    G4double yPosPLYBlock1 = 0.*cm;
    auto vecPLYBlock1 = G4ThreeVector(xPosPLYBlock1,
                                      yPosPLYBlock1,
                                      zPLYBlockPos);
    physPLYBlock1 = new G4PVPlacement(0,
                                      vecPLYBlock1,
                                      logicPLYBlock,
                                      "physPLYBlock1",
                                      logicWorld,
                                      false,
                                      0,
                                      true
                                     );

    // Place Block 2
    G4double xPosPLYBlock2 = -oRadSourceShield - xPLYBlock/2 - 0.1*cm;
    G4double yPosPLYBlock2 = 0.*cm;
    auto vecPLYBlock2 = G4ThreeVector(xPosPLYBlock2,
                                      yPosPLYBlock2,
                                      zPLYBlockPos);
    physPLYBlock2 = new G4PVPlacement(0,
                                      vecPLYBlock2,
                                      logicPLYBlock,
                                      "physPLYBlock2",
                                      logicWorld,
                                      false,
                                      1,
                                      true
                                     ); 

    /* Colours & Styling */
    auto colPLYBlock = G4Colour(1., 0.95, 0.95);
    G4VisAttributes *visAttributesPLYBlock = new G4VisAttributes(colPLYBlock);
    visAttributesPLYBlock->SetVisibility(true);
    visAttributesPLYBlock->SetForceSolid(true);
    visAttributesPLYBlock->SetForceAuxEdgeVisible(true); 
    logicPLYBlock->SetVisAttributes(visAttributesPLYBlock);

}

void MyDetectorConstruction::ConstructCrystals() {

    G4double xCrystal = m_hGeoParams["xCrystal"];
    G4double yCrystal = m_hGeoParams["yCrystal"];
    G4double zCrystal = m_hGeoParams["zCrystal"];
    G4double RadCrystalSource = m_hGeoParams["RadCrystalSource"];

    G4double zPLYWheel    = m_hGeoParams["zPLYWheel"];
    G4double zWheel       = m_hGeoParams["zWheel"];
    G4double zPaper       = m_hGeoParams["zPaper"];
    G4double zProfileLong = m_hGeoParams["zProfileLong"];
    G4double zFloor       = m_hGeoParams["zFloor"];
    G4double zWorld       = m_hGeoParams["zWorld"];

    G4double zWheelEff;
    if (usePLYWheel) {
        zWheelEff = zPLYWheel;
    } else {
        zWheelEff = zWheel + zPaper;
    }

    
    solidCrystal = new G4Box("solidCrystal",
                              xCrystal/2,
                              yCrystal/2,
                              zCrystal/2
                             );

    logicCrystal = new G4LogicalVolume(solidCrystal,
                                        LiF,
                                        "logicCrystal"
                                       );

    G4double zCrystalPos = -zWorld/2
                    + zFloor 
                    + zProfileLong 
                    + zWheelEff 
                    + zCrystal/2;
    auto vecCrystal0 = G4ThreeVector(RadCrystalSource, 0., zCrystalPos);
    physCrystal0 = new G4PVPlacement(0,
                                      vecCrystal0,
                                      logicCrystal,
                                      "physCrystal0",
                                      logicWorld,
                                      false,
                                      0,
                                      true
                                     );

    auto vecCrystal1 = G4ThreeVector(-RadCrystalSource, 0., zCrystalPos);
    physCrystal1 = new G4PVPlacement(0,
                                      vecCrystal1,
                                      logicCrystal,
                                      "physCrystal1",
                                      logicWorld,
                                      false,
                                      1,
                                      true
                                     );

    auto vecCrystal2 = G4ThreeVector(0., RadCrystalSource, zCrystalPos);
    physCrystal2 = new G4PVPlacement(0,
                                      vecCrystal2,
                                      logicCrystal,
                                      "physCrystal2",
                                      logicWorld,
                                      false,
                                      2,
                                      true
                                     );

    auto vecCrystal3 = G4ThreeVector(0., -RadCrystalSource, zCrystalPos);
    physCrystal3 = new G4PVPlacement(0,
                                      vecCrystal3,
                                      logicCrystal,
                                      "physCrystal3",
                                      logicWorld,
                                      false,
                                      3,
                                      true
                                     );
        
    /* Colours & Styling */
    auto colCrystal = G4Colour(1., 1., 0.);
    G4VisAttributes *visAttributesCrystal = new G4VisAttributes(colCrystal);
    visAttributesCrystal->SetVisibility(true);
    visAttributesCrystal->SetForceSolid(true);
    visAttributesCrystal->SetForceAuxEdgeVisible(true); 
    logicCrystal->SetVisAttributes(visAttributesCrystal);

}


void MyDetectorConstruction::ConstructSDandField(){

    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    logicCrystal->SetSensitiveDetector(sensDet);

}



