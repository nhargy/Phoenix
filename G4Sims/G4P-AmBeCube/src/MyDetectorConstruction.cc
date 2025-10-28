#include "MyDetectorConstruction.hh"

MyDetectorConstruction::MyDetectorConstruction() {
    DefineMaterials();
};

MyDetectorConstruction::~MyDetectorConstruction() {
};

/*
G4VPhysicalVolume *MyDetectorConstruction::Construct() {

    DefineGeometryParameters();
    DefineMaterials();
    ConstructLab();
    ConstructFrame();
    ConstructWheel();
    ConstructSourceShield();
    ConstructCrystals();
    ConstructModerators();

    return physWorld;
} */

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

    // ======= Source Shield ======= //
    m_hGeoParams["oRadSourceShield"] = 3.35 *cm;
    m_hGeoParams["iRadSourceShield"] = 2.85 *cm;
    m_hGeoParams["zSourceShield"]    = 5.2  *cm;
    m_hGeoParams["zPlug"]            = 0.85 *cm;

    // ======= Cubes ======= //
    m_hGeoParams["xCube"] = 1. *cm;
    m_hGeoParams["yCube"] = 1. *cm;
    m_hGeoParams["zCube"] = 1. *cm;

    m_hGeoParams["RadCubeSource"] = 10. *cm;
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
    
    // **** Build World ****************************************************
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

    // **** Build Floor ****************************************************
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

    

}

G4VPhysicalVolume *MyDetectorConstruction::Construct() {
    
    // **** Build World ****************************************************
    G4double xWorld = 100.*cm;
    G4double yWorld = 100.*cm;
    G4double zWorld = 200.*cm;

    G4double xWorldPos = 0.*cm;
    G4double yWorldPos = 0.*cm;
    G4double zWorldPos = 0.*cm;
    
    auto WorldVector = G4ThreeVector(xWorldPos, yWorldPos, zWorldPos);    

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
                                   WorldVector,
                                   logicWorld,
                                   "physWorld",
                                   0,
                                   false,
                                   0,
                                   true
                                ); 
    // *********************************************************************


    // **** Build Floor ****************************************************
    G4double xFloor = xWorld;
    G4double yFloor = yWorld;
    G4double zFloor = 20.*cm;

    G4double xFloorPos = 0.*cm;
    G4double yFloorPos = 0.*cm;
    G4double zFloorPos = (-zWorld/2 + zFloor/2);
    
    auto FloorVector = G4ThreeVector(xFloorPos, yFloorPos, zFloorPos);    

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
                                   FloorVector,
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
    // *********************************************************************


    // **** Build Profile  *************************************************
    G4double xProfile = 4. *cm;
    G4double yProfile = 4. *cm;
    G4double zProfile = 99.*cm;

    G4double xProfilePos = 0.*cm;
    G4double yProfilePos = 0.*cm;
    G4double zProfilePos = (-zWorld/2 + zFloor + zProfile/2);
    
    auto ProfileVector = G4ThreeVector(xProfilePos, yProfilePos, zProfilePos);    

    solidProfile = new G4Box("solidProfile",
                           xProfile/2,
                           yProfile/2,
                           zProfile/2); 

    logicProfile = new G4LogicalVolume(solidProfile,
                                    Al,
                                    "logicProfile");

    physProfile  = new G4PVPlacement(0,
                                   ProfileVector,
                                   logicProfile,
                                   "physProfile",
                                   logicWorld,
                                   false,
                                   0,
                                   true); 
    
    /* Colours & Styling */
    G4VisAttributes* visAttributesProfile = new G4VisAttributes(G4Colour(0.9, 0.9, 0.95));
    visAttributesProfile->SetVisibility(true);
    visAttributesProfile->SetForceSolid(true);
    logicProfile->SetVisAttributes(visAttributesProfile);

    // *********************************************************************


    // **** Build Legs *****************************************************
    G4double xLeg = 15.*cm;
    G4double yLeg = 2.*cm;
    G4double zLeg = 2.*cm;

    solidLeg = new G4Box("solidLeg",
                           xLeg,
                           yLeg,
                           zLeg); 

    logicLeg = new G4LogicalVolume(solidLeg,
                                    Al,
                                    "logicLeg");

    G4double xLeg1Pos = xLeg + xProfile;
    G4double yLeg1Pos = 0*cm;
    G4double zLeg1Pos = (-zWorld + 2*zFloor + zLeg);

    auto Leg1Vector = G4ThreeVector(xLeg1Pos, yLeg1Pos, zLeg1Pos);    

    G4RotationMatrix *rot1 = new G4RotationMatrix();
    rot1->rotateZ(0*deg);
 
    physLeg1  = new G4PVPlacement(rot1,
                                   Leg1Vector,
                                   logicLeg,
                                   "physLeg1",
                                   logicWorld,
                                   false,
                                   0,
                                   true); 
 
    G4double xLeg2Pos = 0*cm;
    G4double yLeg2Pos = xLeg + yProfile;
    G4double zLeg2Pos = (-zWorld + 2*zFloor + zLeg);

    auto Leg2Vector = G4ThreeVector(xLeg2Pos, yLeg2Pos, zLeg2Pos);    

    G4RotationMatrix *rot2 = new G4RotationMatrix();
    rot2->rotateZ(90*deg);

    physLeg2  = new G4PVPlacement(rot2,
                                   Leg2Vector,
                                   logicLeg,
                                   "physLeg2",
                                   logicWorld,
                                   false,
                                   1,
                                   true); 
  
    G4double xLeg3Pos = -xLeg - xProfile;
    G4double yLeg3Pos = 0*cm;
    G4double zLeg3Pos = (-zWorld + 2*zFloor + zLeg);

    auto Leg3Vector = G4ThreeVector(xLeg3Pos, yLeg3Pos, zLeg3Pos);    

    G4RotationMatrix *rot3 = new G4RotationMatrix();
    rot3->rotateZ(0*deg);

    physLeg3  = new G4PVPlacement(rot3,
                                   Leg3Vector,
                                   logicLeg,
                                   "physLeg3",
                                   logicWorld,
                                   false,
                                   2,
                                   true); 
  
    G4double xLeg4Pos = 0*cm;
    G4double yLeg4Pos = -xLeg - yProfile;
    G4double zLeg4Pos = (-zWorld + 2*zFloor + zLeg);

    auto Leg4Vector = G4ThreeVector(xLeg4Pos, yLeg4Pos, zLeg4Pos);    

    G4RotationMatrix *rot4 = new G4RotationMatrix();
    rot4->rotateZ(90*deg);

    physLeg4  = new G4PVPlacement(rot4,
                                   Leg4Vector,
                                   logicLeg,
                                   "physLeg4",
                                   logicWorld,
                                   false,
                                   3,
                                   true); 
        
    /* Colours & Styling */
    G4VisAttributes* visAttributesLeg = new G4VisAttributes(G4Colour(0.92, 0.9, 0.85));
    visAttributesLeg->SetVisibility(true);
    visAttributesLeg->SetForceSolid(true);
    logicLeg->SetVisAttributes(visAttributesLeg);

    // *********************************************************************


    // **** Build Rim ******************************************************
    G4double InnerRadRim = 12.   *cm;
    G4double OuterRadRim = 15.   *cm;
    G4double HeightRim   =  0.05 *cm;

    G4double xRimPos = 0.*cm;
    G4double yRimPos = 0.*cm;
    G4double zRimPos = (-zWorld + 2*zFloor + 2*zProfile + HeightRim);
    
    auto RimVector = G4ThreeVector(xRimPos, yRimPos, zRimPos);    

    solidRim = new G4Tubs("solidRim",
                           InnerRadRim,
                           OuterRadRim,
                           HeightRim,
                           0.*deg,
                           360.0*deg); 

    logicRim = new G4LogicalVolume(solidRim,
                                    Al,
                                    "logicRim");

    /*
    physRim  = new G4PVPlacement(0,
                                   RimVector,
                                   logicRim,
                                   "physRim",
                                   logicWorld,
                                   false,
                                   0,
                                   true); */
    
    /* Colours & Styling */
    G4VisAttributes* visAttributesRim = new G4VisAttributes(G4Colour(0.9, 0.95, 0.85));
    visAttributesRim->SetVisibility(true);
    visAttributesRim->SetForceSolid(true);
    logicRim->SetVisAttributes(visAttributesProfile);

    // *********************************************************************


    // **** Build Spine ****************************************************
    G4double xSpine = InnerRadRim + 0.1*cm;
    G4double ySpine = 2.*cm;
    G4double zSpine = 0.05*cm;

    G4double xSpinePos = 0.*cm;
    G4double ySpinePos = 0.*cm;
    G4double zSpinePos = (-zWorld + 2*zFloor + 2*zProfile + zSpine);
    
    auto SpineVector = G4ThreeVector(xSpinePos, ySpinePos, zSpinePos);    

    solidSpine = new G4Box("solidSpine",
                           xSpine,
                           ySpine,
                           zSpine); 

    logicSpine = new G4LogicalVolume(solidSpine,
                                    Al,
                                    "logicSpine");
    /*
    physSpine  = new G4PVPlacement(0,
                                   SpineVector,
                                   logicSpine,
                                   "physSpine",
                                   logicWorld,
                                   false,
                                   0,
                                   true); */
    
    /* Colours & Styling */
    logicSpine->SetVisAttributes(visAttributesRim);

    // *********************************************************************


    // **** Build subSpines ************************************************
    G4double xSubSpine = 2*cm;
    G4double ySubSpine = 4.8*cm;
    G4double zSubSpine = 0.05*cm;

    solidSubSpine = new G4Box("solidSubSpine",
                           xSubSpine,
                           ySubSpine,
                           zSubSpine); 

    logicSubSpine = new G4LogicalVolume(solidSubSpine,
                                    Al,
                                    "logicSubSpine");

    G4double xSubSpine1Pos = 0.*cm;
    G4double ySubSpine1Pos = ySubSpine + ySpine;
    G4double zSubSpine1Pos = (-zWorld + 2*zFloor + 2*zProfile + zSubSpine);
    
    auto SubSpineVector1 = G4ThreeVector(xSubSpine1Pos, ySubSpine1Pos, zSubSpine1Pos);    

    /*
    physSubSpine1  = new G4PVPlacement(0,
                                   SubSpineVector1,
                                   logicSubSpine,
                                   "physSubSpine",
                                   logicWorld,
                                   false,
                                   0,
                                   true); 
 
    G4double xSubSpine2Pos = 0.*cm;
    G4double ySubSpine2Pos = -ySubSpine - ySpine;
    G4double zSubSpine2Pos = (-zWorld + 2*zFloor + 2*zProfile + zSubSpine);
    
    auto SubSpineVector2 = G4ThreeVector(xSubSpine2Pos, ySubSpine2Pos, zSubSpine2Pos);   */ 

    /*
    physSubSpine2  = new G4PVPlacement(0,
                                   SubSpineVector2,
                                   logicSubSpine,
                                   "physSubSpine",
                                   logicWorld,
                                   false,
                                   1,
                                   true); */
       
    /* Colours & Styling */
    logicSubSpine->SetVisAttributes(visAttributesRim);

    // *********************************************************************

    
    // **** Build Wheel Union **********************************************
    G4RotationMatrix* RotFlip = new G4RotationMatrix;
    RotFlip->rotateZ(90*degree);

    G4RotationMatrix* RotWheel = new G4RotationMatrix;
    RotWheel->rotateZ(45*degree);

    G4ThreeVector ZeroTrans(0., 0., 0.);
    G4ThreeVector Trans(xSpinePos, ySpinePos, zSpinePos);
    
    G4UnionSolid* solidUnion1 = new G4UnionSolid("SolidRim+iSolidSpine1", 
                                  solidRim, 
                                  solidSpine,
                                  0,
                                  ZeroTrans
                                  );

    G4UnionSolid* solidWheel = new G4UnionSolid("Wheel", 
                                  solidUnion1, 
                                  solidSpine,
                                  RotFlip,
                                  ZeroTrans
                                  );

    G4LogicalVolume* logicWheel = new G4LogicalVolume(solidWheel,
                                                        Al,
                                                        "logicWheel"
                                                        );

    G4VPhysicalVolume *physWheel = new G4PVPlacement(RotWheel,
                                                       RimVector,
                                                        logicWheel,
                                                        "physWheel",
                                                        logicWorld,
                                                        false,
                                                        0,
                                                        true);

    /* Colours & Styling */
    logicWheel->SetVisAttributes(visAttributesRim);
    
    // *********************************************************************

    
    // **** Build Paper ****************************************************

    G4double paperThickness = 0.1*mm;
    
    G4Tubs* solidPaper = new G4Tubs("solidPaper",
                                    0. *cm,
                                    OuterRadRim,
                                    (paperThickness/2) *mm,
                                    0.   *deg,
                                    360. *deg
                                    );

    G4LogicalVolume* logicPaper = new G4LogicalVolume(solidPaper,
                                                      Paper,
                                                      "logicPaper");
    G4double xPaperPos = 0.*cm;
    G4double yPaperPos = 0.*cm;
    G4double zPaperPos = (zRimPos + HeightRim + (paperThickness/2)*mm);
    
    auto PaperVector = G4ThreeVector(xPaperPos, yPaperPos, zPaperPos);
    G4VPhysicalVolume* physPaper = new G4PVPlacement(0,
                                                      PaperVector,
                                                        logicPaper,
                                                        "physPaper",
                                                        logicWorld,
                                                        false,
                                                        0,
                                                        true);

    /* Colours & Styling */
    G4VisAttributes* visAttributesPaper = new G4VisAttributes(G4Colour(1., 1., 1., 1.));
    visAttributesPaper->SetVisibility(true);
    visAttributesPaper->SetForceSolid(true);
    logicPaper->SetVisAttributes(visAttributesPaper);

    // *********************************************************************

    
    // **** Build Cylinder *************************************************
    G4double InnerRadCylinder = 1.68*cm;
    G4double OuterRadCylinder = 3.35*cm;
    G4double HeightCylinder   = 5.2*cm;

    G4double xCylinderPos = 0.*cm;
    G4double yCylinderPos = 0.*cm;
    G4double zCylinderPos = (zPaperPos + paperThickness/2 + HeightCylinder);
    //G4double zCylinderPos = (-zWorld + 2*zFloor + 2*zProfile + 2*HeightRim + HeightCylinder);
    
    auto CylinderVector = G4ThreeVector(xCylinderPos, yCylinderPos, zCylinderPos);    

    solidCylinder = new G4Tubs("solidCylinder",
                           InnerRadCylinder,
                           OuterRadCylinder,
                           HeightCylinder,
                           0.*deg,
                           360.0*deg); 

    logicCylinder = new G4LogicalVolume(solidCylinder,
                                    Pb,
                                    "logicCylinder");

    physCylinder  = new G4PVPlacement(0,
                                   CylinderVector,
                                   logicCylinder,
                                   "physCylinder",
                                   logicWorld,
                                   false,
                                   0,
                                   true); 
    
    /* Colours & Styling */
    G4VisAttributes* visAttributesCylinder = new G4VisAttributes(G4Colour(0.2, 0.1, 0.1, 0.9));
    visAttributesCylinder->SetVisibility(true);
    visAttributesCylinder->SetForceSolid(true);
    logicCylinder->SetVisAttributes(visAttributesCylinder);

    // *********************************************************************


    // **** Build Plugs ** *************************************************
    G4double InnerRadPlug = 0.*cm;
    G4double OuterRadPlug = 1.68*cm;
    G4double HeightPlug   = 0.85*cm;

    solidPlug = new G4Tubs("solidPlug",
                           InnerRadPlug,
                           OuterRadPlug,
                           HeightPlug,
                           0.*deg,
                           360.0*deg); 

    logicPlug = new G4LogicalVolume(solidPlug,
                                    Pb,
                                    "logicPlug");

    G4double xPlug1Pos = 0.*cm;
    G4double yPlug1Pos = 0.*cm;
    G4double zPlug1Pos = (zCylinderPos + HeightCylinder - HeightPlug);
    
    auto Plug1Vector = G4ThreeVector(xPlug1Pos, yPlug1Pos, zPlug1Pos);    

    physPlug1  = new G4PVPlacement(0,
                                   Plug1Vector,
                                   logicPlug,
                                   "physPlug1",
                                   logicWorld,
                                   false,
                                   0,
                                   true); 
 
    G4double xPlug2Pos = 0.*cm;
    G4double yPlug2Pos = 0.*cm;
    G4double zPlug2Pos = (zCylinderPos - HeightCylinder + HeightPlug);
    
    auto Plug2Vector = G4ThreeVector(xPlug2Pos, yPlug2Pos, zPlug2Pos);    

    physPlug2  = new G4PVPlacement(0,
                                   Plug2Vector,
                                   logicPlug,
                                   "physPlug2",
                                   logicWorld,
                                   false,
                                   1,
                                   true);    

    /* Colours & Styling */
    logicPlug->SetVisAttributes(visAttributesCylinder);

    // *********************************************************************

    
    // **** Build Cubes ****************************************************
    G4RotationMatrix* RotZ = new G4RotationMatrix;
    RotZ->rotateZ(0*degree);

    G4double radAway = 10 *cm;
    
    G4double xCube = 1. *cm;
    G4double yCube = 1. *cm;
    G4double zCube = 1. *cm;
    
    solidCube = new G4Box("solidCube",
                           xCube/2,
                           yCube/2,
                           zCube/2); 

    logicCube = new G4LogicalVolume(solidCube,
                                    LiF,
                                    "logicCube");
    
    G4double z = zPaperPos + paperThickness/2 + zCube/2 ;
    
    G4double x1 = radAway;
    G4double y1 = 0. *cm;

    auto vecCube1 = G4ThreeVector(x1, y1, z);    
    physCube1 = new G4PVPlacement(RotZ,
                                   vecCube1,
                                   logicCube,
                                   "physCube1",
                                   logicWorld,
                                   false,
                                   0,
                                   true);

    G4double x2 = 0.;
    G4double y2 = radAway;

    auto vecCube2 = G4ThreeVector(x2, y2, z);    
    physCube2 = new G4PVPlacement(RotZ,
                                   vecCube2,
                                   logicCube,
                                   "physCube2",
                                   logicWorld,
                                   false,
                                   1,
                                   true);


    G4double x3 = -radAway;
    G4double y3 = 0. *cm;

    auto vecCube3 = G4ThreeVector(x3, y3, z);    
    physCube3 = new G4PVPlacement(RotZ,
                                   vecCube3,
                                   logicCube,
                                   "physCube3",
                                   logicWorld,
                                   false,
                                   2,
                                   true);

    G4double x4 = 0 *cm;
    G4double y4 = -radAway;

    auto vecCube4 = G4ThreeVector(x4, y4, z);    
    physCube4 = new G4PVPlacement(RotZ,
                                   vecCube4,
                                   logicCube,
                                   "physCube4",
                                   logicWorld,
                                   false,
                                   3,
                                   true);
    
    /* Colours & Styling */
    G4VisAttributes* visAttributesCube = new G4VisAttributes(G4Colour(1., 1., 0., 0.8));
    visAttributesCube->SetVisibility(true);
    visAttributesCube->SetForceSolid(true);
    logicCube->SetVisAttributes(visAttributesCube);

    // *********************************************************************


    // **** Build Water ****************************************************
    /*
    G4RotationMatrix* RotZWater = new G4RotationMatrix;
    RotZWater->rotateZ(45*degree);
    
    G4double xWater = 2 *cm;
    G4double yWater = 2 *cm;
    G4double zWater = 3.  *cm;

    solidWater = new G4Box("solidWater",
                           xWater,
                           yWater,
                           zWater); 

    logicWater = new G4LogicalVolume(solidWater,
                                    Water,
                                    "logicWater");

    G4double xWaterPos = 4.*cm;
    G4double yWaterPos = 4.*cm;
    G4double zWaterPos = zRimPos + HeightRim + zWater ;
    
    
    auto WaterVector = G4ThreeVector(xWaterPos, yWaterPos, zWaterPos);    
    physWater  = new G4PVPlacement(RotZWater,
                                   WaterVector,
                                   logicWater,
                                   "physWater",
                                   logicWorld,
                                   false,
                                   0,
                                   true); */
    
    /* Colours & Styling */
    /*
    G4VisAttributes* visAttributesWater = new G4VisAttributes(G4Colour(0., 0., 1., 0.3));
    visAttributesWater->SetVisibility(true);
    visAttributesWater->SetForceSolid(true);
    logicWater->SetVisAttributes(visAttributesWater); */

    // *********************************************************************
    
    return physWorld;
};

void MyDetectorConstruction::ConstructSDandField(){

    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    logicCube->SetSensitiveDetector(sensDet);

}



