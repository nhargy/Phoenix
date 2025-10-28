#include "MyConstruction.hh"

MyDetectorConstruction::MyDetectorConstruction(){

    fMessengerCube = new G4GenericMessenger(this,
                                        "/MyCube/",
                                        "MyCube");

    fMessengerCube->DeclareProperty("CubeDistance",
                                CubeDistance,
                                "Distance of cube from source");
 
    fMessengerCube->DeclareProperty("CubeSide",
                                CubeSide,
                                "Side dimensions of the cube");
       
    CubeDistance = 4.0;
    CubeSide     = 0.5;

    DefineMaterials();
}

MyDetectorConstruction::~MyDetectorConstruction(){}

void MyDetectorConstruction::DefineMaterials(){

    G4NistManager *nist = G4NistManager::Instance(); 
    
    Air = nist->FindOrBuildMaterial("G4_AIR"); 
    LiF = nist->FindOrBuildMaterial("G4_LITHIUM_FLUORIDE");
    Pb  = nist->FindOrBuildMaterial("G4_Pb");
    Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");

    PLA = new G4Material("PLA", 1.25*g/cm3, 3);
    PLA->AddElement(nist->FindOrBuildElement("C"), 3);
    PLA->AddElement(nist->FindOrBuildElement("H"), 4);
    PLA->AddElement(nist->FindOrBuildElement("O"), 2);

}

G4VPhysicalVolume *MyDetectorConstruction::Construct(){
   
    // Build World 
    G4double xWorld = 0.5*m;
    G4double yWorld = 0.5*m;
    G4double zWorld = 0.5*m;

    solidWorld = new G4Box("solidWorld",
                           xWorld,
                           yWorld,
                           zWorld); 

    logicWorld = new G4LogicalVolume(solidWorld,
                                    Air,
                                    "logicWorld");

    physWorld  = new G4PVPlacement(0,
                                   G4ThreeVector(0.,0.,0.),
                                   logicWorld,
                                   "physWorld",
                                   0,
                                   false,
                                   0,
                                   true); 

    // Build Holder
    G4VisAttributes* visAttributesHolder = new G4VisAttributes(G4Colour(0.9, 0., 0.9));
    visAttributesHolder->SetVisibility(true);
    visAttributesHolder->SetForceSolid(true);

    G4double holderInnerRadius = 0*cm;
    G4double holderOuterRadius = 5*cm;
    G4double holderHeight      = 0.4*cm;

    solidHolder = new G4Tubs("solidholder",
                               holderInnerRadius,
                               holderOuterRadius,
                               holderHeight,
                               0.*deg,
                               360.*deg
                              );

    logicHolder = new G4LogicalVolume(solidHolder,
                                      PLA,
                                      "logicHolder");

    physHolder = new G4PVPlacement(0,
                                   G4ThreeVector(0.,0., holderHeight-20*cm),
                                   logicHolder,
                                   "physHolder",
                                   logicWorld,
                                   false,
                                   0,
                                   true); 

    
    logicHolder->SetVisAttributes(visAttributesHolder);

    solidTube = new G4Tubs("solidTube",
                           3.3*mm,
                           5.5*mm,
                           9*mm,
                           0.*deg,
                           360.*deg);

    logicTube = new G4LogicalVolume(solidTube,
                                    PLA,
                                    "logicTube");

    physTube = new G4PVPlacement(0,
                                 G4ThreeVector(0., 0., (2*holderHeight)-20*cm+9*mm),
                                 logicTube,
                                 "logicTube",
                                 logicWorld,
                                 false,
                                 0,
                                 true);

    logicTube->SetVisAttributes(visAttributesHolder);


    // Build Blocks

    G4VisAttributes* visAttributesBlock = new G4VisAttributes(G4Colour(0.6, 0.6, 1.));
    visAttributesBlock->SetVisibility(true);
    visAttributesBlock->SetForceWireframe(true);
    visAttributesBlock->SetForceAuxEdgeVisible(true);

    solidBlockBottom = new G4Box("solidBlockBottom",
                            10*cm,
                            10*cm,
                            2.5*cm);

    logicBlockBottom = new G4LogicalVolume(solidBlockBottom,
                                     Pb,
                                     "logicBlockBottom");

    physBlockBottom = new G4PVPlacement(0,
                                   G4ThreeVector(0,0,-22.5*cm),
                                   logicBlockBottom,
                                   "physBlockBottom",
                                   logicWorld,
                                   false,
                                   0,
                                   true); 

    logicBlockBottom->SetVisAttributes(visAttributesBlock);


    solidBlockSide1 = new G4Box("solidBlockSide1",
                            2.5*cm,
                            12.5*cm,
                            15*cm);

    logicBlockSide1 = new G4LogicalVolume(solidBlockSide1,
                                     Pb,
                                     "logicBlockSide1");

    G4RotationMatrix *rot1 = new G4RotationMatrix();
    rot1->rotateZ(0*deg);
    physBlockSide1 = new G4PVPlacement(rot1,
                                   G4ThreeVector(12.5*cm,2.5*cm,-10*cm),
                                   logicBlockSide1,
                                   "physBlockSide1",
                                   logicWorld,
                                   false,
                                   0,
                                   true); 

    logicBlockSide1->SetVisAttributes(visAttributesBlock);


    solidBlockSide2 = new G4Box("solidBlockSide2",
                            2.5*cm,
                            12.5*cm,
                            15*cm);

    logicBlockSide2 = new G4LogicalVolume(solidBlockSide2,
                                     Pb,
                                     "logicBlockSide2");

    G4RotationMatrix *rot2 = new G4RotationMatrix();
    rot2->rotateZ(90*deg);
    physBlockSide2 = new G4PVPlacement(rot2,
                                   G4ThreeVector(2.5*cm,-12.5*cm,-10*cm),
                                   logicBlockSide2,
                                   "physBlockSide2",
                                   logicWorld,
                                   false,
                                   0,
                                   true); 

    logicBlockSide2->SetVisAttributes(visAttributesBlock);


    solidBlockSide3 = new G4Box("solidBlockSide3",
                            2.5*cm,
                            12.5*cm,
                            15*cm);

    logicBlockSide3 = new G4LogicalVolume(solidBlockSide3,
                                     Pb,
                                     "logicBlockSide3");

    G4RotationMatrix *rot3 = new G4RotationMatrix();
    rot3->rotateZ(0*deg);
    physBlockSide3 = new G4PVPlacement(rot3,
                                   G4ThreeVector(-12.5*cm,-2.5*cm,-10*cm),
                                   logicBlockSide3,
                                   "physBlockSide3",
                                   logicWorld,
                                   false,
                                   0,
                                   true); 

    logicBlockSide3->SetVisAttributes(visAttributesBlock);


    solidBlockSide4 = new G4Box("solidBlockSide4",
                            2.5*cm,
                            12.5*cm,
                            15*cm);

    logicBlockSide4 = new G4LogicalVolume(solidBlockSide4,
                                     Pb,
                                     "logicBlockSide4");

    G4RotationMatrix *rot4 = new G4RotationMatrix();
    rot4->rotateZ(90*deg);
    physBlockSide4 = new G4PVPlacement(rot4,
                                   G4ThreeVector(-2.5*cm,12.5*cm,-10*cm),
                                   logicBlockSide4,
                                   "physBlockSide4",
                                   logicWorld,
                                   false,
                                   0,
                                   true); 

    logicBlockSide4->SetVisAttributes(visAttributesBlock);


    // Build Cylinder

    solidCylinder = new G4Tubs("solidCylinder",
                           6.5*cm,
                           7.5*cm,
                           10*cm,
                           0.*deg,
                           360.*deg);

    logicCylinder = new G4LogicalVolume(solidCylinder,
                                    Pb,
                                    "logicCylinder");

    physCylinder = new G4PVPlacement(0,
                                 G4ThreeVector(0., 0., -20*cm+10*cm),
                                 logicCylinder,
                                 "logicCylinder",
                                 logicWorld,
                                 false,
                                 0,
                                 true);

    logicCylinder->SetVisAttributes(visAttributesBlock);




    // Build Floor

    G4VisAttributes* visAttributesFloor = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3, 0.9));
    visAttributesFloor->SetVisibility(true);
    visAttributesFloor->SetForceSolid(true);


    solidFloor = new G4Box("solidFloor",
                           xWorld,
                           yWorld,
                           0.125*m
                          );

    logicFloor = new G4LogicalVolume(solidFloor,
                                     Concrete,
                                     "logicFloor"
                                    );

    physFloor = new G4PVPlacement(0,
                                  G4ThreeVector(0., 0., -0.375*m),
                                  logicFloor,
                                  "physFloor",
                                  logicWorld,
                                  false,
                                  0,
                                  true
                                 );

    logicFloor->SetVisAttributes(visAttributesFloor);


    // Build Cube

    G4VisAttributes* visAttributesCube = new G4VisAttributes(G4Colour(1, 1, 1, 0.8));
    visAttributesCube->SetVisibility(true);
    visAttributesCube->SetForceSolid(true);


    solidCube = new G4Box("solidCube",
                          CubeSide*cm,
                          CubeSide*cm,
                          CubeSide*cm);

    logicCube = new G4LogicalVolume(solidCube,
                                    LiF,
                                    "logicCube");

    physCube = new G4PVPlacement(0,
                                 G4ThreeVector(CubeDistance*cm, 0., -20*cm + (2*holderHeight) + CubeSide*cm ),
                                 logicCube,
                                 "logicCube",
                                 logicWorld,
                                 false,
                                 0,
                                 true);

    logicCube->SetVisAttributes(visAttributesCube);


    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField(){

    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    logicCube->SetSensitiveDetector(sensDet);

}

