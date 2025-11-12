#include "MyDetectorConstruction.hh"
#include <fstream>
#include <string>

map<G4String, G4double> MyDetectorConstruction::m_hGeoParams;

MyDetectorConstruction::MyDetectorConstruction(const G4String& outputPath) : fOutputDirectory(outputPath) {
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

    return phys_Lab;
}

void MyDetectorConstruction::DefineGeoParams() {

    // ======= Lab ======= //
    m_hGeoParams["Lab_Width"]  = 100. *cm;
    m_hGeoParams["Lab_Length"] = 100. *cm;
    m_hGeoParams["Lab_Height"] = 200. *cm;

    m_hGeoParams["Floor_Height"] =  20. *cm;

    // ======= Profiles ======= //
    m_hGeoParams["Profile_Width"]       =  4. *cm;
    m_hGeoParams["Profile_Length"]      =  4. *cm;
    m_hGeoParams["ProfileLong_Height"]  = 99. *cm;
    m_hGeoParams["ProfileShort_Height"] = 30. *cm;

    // ======= Wheel ======= //
    m_hGeoParams["Wheel_oRadius"] = 15.   *cm;
    m_hGeoParams["Wheel_iRadius"] = 12.   *cm;
    m_hGeoParams["Wheel_Height"]  =  0.05 *cm;
    m_hGeoParams["Spoke_Width"]   = 2. *cm;

    m_hGeoParams["Paper_Height"]  = 0.01 *cm;

    // ======= PLY Wheel ======= //
    m_hGeoParams["PLYWheel_oRadius"] = 15.  *cm;
    m_hGeoParams["PLYWheel_iRadius"] =  0.  *cm;
    m_hGeoParams["PLYWheel_Height"]  =  1.5 *cm;

    // ======= PLY Blocks ======= //
    m_hGeoParams["PLYBlock_Width"]  =  5. *cm;
    m_hGeoParams["PLYBlock_Length"] = 20. *cm;
    m_hGeoParams["PLYBlock_Height"] = 10. *cm;

    // ======= Source Shield ======= //
    m_hGeoParams["SourceShield_oRadius"] = 3.35 *cm;
    m_hGeoParams["SourceShield_iRadius"] = 2.85 *cm;
    m_hGeoParams["SourceShield_Height"]  = 10.2 *cm;
    m_hGeoParams["Plug_Height"]          = 1.7  *cm;

    // ======= Crystals ======= //
    m_hGeoParams["Crystal_Width"]  = 1. *cm;
    m_hGeoParams["Crystal_Length"] = 1. *cm;
    m_hGeoParams["Crystal_Height"] = 1. *cm;

    m_hGeoParams["CrystalSource_Distance"] = 10. *cm;

    // After parameters are defined, write them to CSV (if requested)
    SaveGeoParamsToCSV();
};

void MyDetectorConstruction::SaveGeoParamsToCSV() const {
    // Build path
    std::string outDir = std::string(fOutputDirectory);
    if (!outDir.empty() && outDir.back() != '/') outDir.push_back('/');
    std::string filePath = outDir + "geo_params.csv";

    std::ofstream fout(filePath);
    if (!fout.is_open()) {
        // Could optionally print a message to G4cout, but be quiet for now
        return;
    }

    // Header
    fout << "key,value\n";

    for (const auto &kv : m_hGeoParams) {
        // Write key and value (value in internal units, e.g. cm)
        fout << kv.first << "," << kv.second << "\n";
    }

    fout.close();
}

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
    
    // **** Build Lab **** //
    G4double Lab_Width  = m_hGeoParams["Lab_Width"];
    G4double Lab_Length = m_hGeoParams["Lab_Length"];
    G4double Lab_Height = m_hGeoParams["Lab_Height"];

    solid_Lab = new G4Box(
        "solidWorld",
        Lab_Width/2,
        Lab_Length/2,
        Lab_Height/2
    );

    logic_Lab = new G4LogicalVolume(
        solid_Lab,
        Air,
        "logic_Lab"
    );

    auto Lab_Vec = G4ThreeVector(0., 0., 0.);
    phys_Lab  = new G4PVPlacement(
        0,
        Lab_Vec,
        logic_Lab,
        "phys_Lab",
        0,
        false,
        0,
        true
    );

    // **** Build Floor **** //
    G4double Floor_Width  = Lab_Width;
    G4double Floor_Length = Lab_Length;
    G4double Floor_Height = m_hGeoParams["Floor_Height"];
  
    solid_Floor = new G4Box(
        "solidFloor",
        Floor_Width/2,
        Floor_Length/2,
        Floor_Height/2
    );

    logic_Floor = new G4LogicalVolume(
        solid_Floor,
        Concrete,
        "logic_Floor"
    );

    G4double x_Floor = 0.*cm;
    G4double y_Floor = 0.*cm;
    G4double z_Floor = (-Lab_Height/2 + Floor_Height/2);
    auto Floor_Vec = G4ThreeVector(x_Floor, y_Floor, z_Floor);  
    phys_Floor  = new G4PVPlacement(0,
                                   Floor_Vec,
                                   logic_Floor,
                                   "phys_Floor",
                                   logic_Lab,
                                   false,
                                   0,
                                   true
                                ); 
    
    /* VisAttributes */
    auto Floor_Colour = G4Colour(0.5, 0.5, 0.5);
    G4VisAttributes *vis_Floor = new G4VisAttributes(Floor_Colour);
    vis_Floor  ->SetVisibility(true);
    vis_Floor  ->SetForceSolid(true);
    logic_Floor->SetVisAttributes(vis_Floor);
};

void MyDetectorConstruction::ConstructFrame() {

    G4double Lab_Height   = m_hGeoParams["Lab_Height"];
    G4double Floor_Height = m_hGeoParams["Floor_Height"];

    G4double Profile_Width       = m_hGeoParams["Profile_Width"];
    G4double Profile_Length      = m_hGeoParams["Profile_Length"];
    G4double ProfileLong_Height  = m_hGeoParams["ProfileLong_Height"];
    G4double ProfileShort_Height = m_hGeoParams["ProfileShort_Height"];


    G4Box *solid_ProfileLong = new G4Box(
        "solid_ProfileLong",
        Profile_Width/2,
        Profile_Length/2,
        ProfileLong_Height/2
    );

    G4Box *solid_ProfileShort = new G4Box(
        "solid_ProfileShort",
        Profile_Width/2,
        Profile_Length/2,
        ProfileShort_Height/2
    );

    // Add Leg1
    G4RotationMatrix* Temp1_Rot = new G4RotationMatrix;
    Temp1_Rot->rotateY(90*deg);
    
    G4double x_Temp1 = Profile_Width/2 + ProfileShort_Height/2; 
    G4double y_Temp1 = 0. *cm;
    G4double z_Temp1 = -ProfileLong_Height/2 + Profile_Width/2;
    auto Temp1_Vec = G4ThreeVector(x_Temp1, y_Temp1, z_Temp1);
    G4VSolid *solid_Temp1 = new G4UnionSolid("solid_Temp1",
                                            solid_ProfileLong,
                                            solid_ProfileShort,
                                            Temp1_Rot,
                                            Temp1_Vec
                                        );

    // Add Leg2
    G4RotationMatrix* Temp2_Rot = new G4RotationMatrix;
    Temp2_Rot->rotateY(90*deg);

    G4double x_Temp2 = -(Profile_Width/2 + ProfileShort_Height/2); 
    G4double y_Temp2 = 0. *cm;
    G4double z_Temp2 = -ProfileLong_Height/2 + Profile_Width/2;
    auto Temp2_Vec = G4ThreeVector(x_Temp2, y_Temp2, z_Temp2);
    G4VSolid *solid_Temp2 = new G4UnionSolid("solid_Temp2",
                                            solid_Temp1,
                                            solid_ProfileShort,
                                            Temp2_Rot,
                                            Temp2_Vec
                                        );

    // Add Leg3
    G4RotationMatrix* Temp3_Rot = new G4RotationMatrix;
    Temp3_Rot->rotateX(90*deg);

    G4double x_Temp3 = 0. *cm; 
    G4double y_Temp3 = Profile_Width/2 + ProfileShort_Height/2;
    G4double z_Temp3 = -ProfileLong_Height/2 + Profile_Width/2;
    auto Temp3_Vec = G4ThreeVector(x_Temp3, y_Temp3, z_Temp3);
    G4VSolid *solid_Temp3 = new G4UnionSolid("solid_Temp3",
                                            solid_Temp2,
                                            solid_ProfileShort,
                                            Temp3_Rot,
                                            Temp3_Vec
                                        );

    // Add Leg4
    G4RotationMatrix* Temp4_Rot = new G4RotationMatrix;
    Temp4_Rot->rotateX(90*deg);

    G4double x_Temp4 = 0. *cm; 
    G4double y_Temp4 = -(Profile_Width/2 + ProfileShort_Height/2);
    G4double z_Temp4 = -ProfileLong_Height/2 + Profile_Width/2;
    auto Temp4_Vec = G4ThreeVector(x_Temp4, y_Temp4, z_Temp4);
    G4VSolid* solid_Frame = new G4UnionSolid("solid_Frame",
                                  solid_Temp3,
                                  solid_ProfileShort,
                                  Temp4_Rot,
                                  Temp4_Vec
                                );

    logic_Frame = new G4LogicalVolume(solid_Frame,
                                     Al,
                                     "logic_Frame"
                                    );

    G4double z_Frame = -Lab_Height/2 + Floor_Height + ProfileLong_Height/2;
    auto Frame_Vec = G4ThreeVector(0., 0., z_Frame);
    G4PVPlacement *phys_Frame = new G4PVPlacement(0,
                                                 Frame_Vec,
                                                 logic_Frame,
                                                 "phys_Frame",
                                                 logic_Lab,
                                                 false,
                                                 0,
                                                 true
                                                 );

    /* VisAttributes */
    auto Frame_Colour = G4Colour(0., 0.8, 1.);
    G4VisAttributes *visFrame = new G4VisAttributes(Frame_Colour);
    visFrame   ->SetVisibility(true);
    visFrame   ->SetForceSolid(true);
    logic_Frame->SetVisAttributes(visFrame);
};

void MyDetectorConstruction::ConstructWheel() {

    G4double Wheel_oRadius = m_hGeoParams["Wheel_oRadius"];
    G4double Wheel_iRadius = m_hGeoParams["Wheel_iRadius"];
    G4double Wheel_Height  = m_hGeoParams["Wheel_Height"];
    G4double Spoke_Width   = m_hGeoParams["Spoke_Width"];

    G4double Paper_Height = m_hGeoParams["Paper_Height"];

    G4double Lab_Height          = m_hGeoParams["Lab_Height"];
    G4double Floor_Height       = m_hGeoParams["Floor_Height"];
    G4double ProfileLong_Height = m_hGeoParams["ProfileLong_Height"];


    G4Tubs *solid_Rim = new G4Tubs(
        "solid_Rim",
        Wheel_iRadius,
        Wheel_oRadius,
        Wheel_Height/2,
        0,
        360 *deg
    );

    G4Tubs *solid_Paper = new G4Tubs(
        "solid_Paper",
        0.,
        Wheel_oRadius,
        Paper_Height/2,
        0.,
        360* deg
    );

    G4Box *solid_Spoke = new G4Box(
        "solid_Spoke",
        Wheel_iRadius + 0.1*cm,
        Spoke_Width/2,
        Wheel_Height/2
    );

    auto Temp1_Vec = G4ThreeVector(0,0,0);
    G4UnionSolid *solid_Temp1 = new G4UnionSolid(
        "solid_Temp1",
        solid_Rim,
        solid_Spoke,
        0,
        Temp1_Vec
    );

    G4RotationMatrix* Temp2_Rot = new G4RotationMatrix;
    Temp2_Rot->rotateZ(90*deg);

    auto Temp2_Vec = G4ThreeVector(0,0,0);
    G4UnionSolid *solid_Temp2 = new G4UnionSolid(
        "solid_Temp2",
        solid_Temp1,
        solid_Spoke,
        Temp2_Rot, 
        Temp2_Vec
    );

    auto Temp3_Vec = G4ThreeVector(0., 0., Wheel_Height/2+Paper_Height/2);
    G4UnionSolid *solid_Wheel = new G4UnionSolid(
        "solid_Wheel",
        solid_Temp2,
        solid_Paper,
        nullptr,
        Temp3_Vec
    );

    logic_Wheel = new G4LogicalVolume(
        solid_Wheel,
        Al,
        "logic_Wheel"
    );


    G4RotationMatrix* Wheel_Rot = new G4RotationMatrix;
    Wheel_Rot->rotateZ(45*deg);

    G4double Wheel_z = -Lab_Height/2 
                        + Floor_Height
                        + ProfileLong_Height
                        + Wheel_Height/2
                        + Paper_Height/2;
    auto Wheel_Vec = G4ThreeVector(0., 0., Wheel_z);        
    phys_Wheel = new G4PVPlacement(
        Wheel_Rot,
        Wheel_Vec,
        logic_Wheel,
        "logic_Wheel",
        logic_Lab,
        false,
        0,
        true
    ); 

    /* VisAttributes */
    auto Wheel_Colour = G4Colour(0.7, 0.7, 0.7);
    G4VisAttributes *vis_Wheel = new G4VisAttributes(Wheel_Colour);
    vis_Wheel  ->SetVisibility(true);
    vis_Wheel  ->SetForceSolid(true);
    logic_Wheel->SetVisAttributes(vis_Wheel);

}

void MyDetectorConstruction::ConstructPLYWheel() {

    G4double PLYWheel_oRadius = m_hGeoParams["PLYWheel_oRadius"];
    G4double PLYWheel_iRadius = m_hGeoParams["PLYWheel_iRadius"];
    G4double PLYWheel_Height  = m_hGeoParams["PLYWheel_Height"];

    G4double ProfileLong_Height = m_hGeoParams["ProfileLong_Height"];
    G4double Floor_Height       = m_hGeoParams["Floor_Height"];
    G4double Lab_Height         = m_hGeoParams["Lab_Height"];

    G4Tubs* solid_PLYWheel = new G4Tubs(
        "solid_PLYWheel",
        PLYWheel_iRadius,
        PLYWheel_oRadius,
        PLYWheel_Height/2,
        0,
        360*deg
    );

    logic_PLYWheel = new G4LogicalVolume(
        solid_PLYWheel,
        Polyethylene,
        "logic_PLYWheel"
    );

    G4double z_PLYWheel = -Lab_Height/2 
                            + Floor_Height 
                            + ProfileLong_Height 
                            + PLYWheel_Height/2;
    auto PLYWheel_Vec = G4ThreeVector(0., 0., z_PLYWheel);
    phys_PLYWheel = new G4PVPlacement(
        0,
        PLYWheel_Vec,
        logic_PLYWheel,
        "phys_PLYWheel",
        logic_Lab,
        false,
        0,
        true
    );

    /* VisAttributes */
    auto PLYWheel_Colour = G4Colour(1., 0.9, 0.9);
    G4VisAttributes *vis_PLYWheel = new G4VisAttributes(PLYWheel_Colour);
    vis_PLYWheel  ->SetVisibility(true);
    vis_PLYWheel  ->SetForceSolid(true);
    logic_PLYWheel->SetVisAttributes(vis_PLYWheel);
}

void MyDetectorConstruction::ConstructSourceShield() {

    G4double SourceShield_oRadius = m_hGeoParams["SourceShield_oRadius"];
    G4double SourceShield_iRadius = m_hGeoParams["SourceShield_iRadius"];
    G4double SourceShield_Height  = m_hGeoParams["SourceShield_Height"];
    G4double Plug_Height          = m_hGeoParams["Plug_Height"];

    G4double PLYWheel_Height    = m_hGeoParams["PLYWheel_Height"];
    G4double Wheel_Height       = m_hGeoParams["Wheel_Height"];
    G4double Paper_Height       = m_hGeoParams["Paper_Height"];
    G4double ProfileLong_Height = m_hGeoParams["ProfileLong_Height"];
    G4double Floor_Height       = m_hGeoParams["Floor_Height"];
    G4double Lab_Height         = m_hGeoParams["Lab_Height"];

    G4double WheelEff_Height;
    if (usePLYWheel) {
        WheelEff_Height = PLYWheel_Height;
    } else {
        WheelEff_Height = Wheel_Height + Paper_Height;
    }


    G4Tubs *solid_Cylinder = new G4Tubs(
        "solid_Cylinder",
        SourceShield_iRadius,
        SourceShield_oRadius,
        SourceShield_Height/2,
        0,
        360*deg
    );

    G4Tubs *solid_Plug = new G4Tubs(
        "solid_Plug",
        0.,
        SourceShield_iRadius + 0.1*cm,
        Plug_Height/2,
        0,
        360*deg
    );

    auto Temp1_Vec = G4ThreeVector(0., 0., (SourceShield_Height/2 - Plug_Height/2));
    G4UnionSolid *solid_Temp1 = new G4UnionSolid(
        "solid_Temp1",
        solid_Cylinder,
        solid_Plug,
        0,
        Temp1_Vec
    );

    auto Temp2_Vec = G4ThreeVector(0., 0., (-SourceShield_Height/2 + Plug_Height/2));
    G4UnionSolid *solid_SourceShield = new G4UnionSolid(
        "solid_SourceShield",
        solid_Temp1,
        solid_Plug,
        0,
        Temp2_Vec
    ); 
                                    
    logic_SourceShield = new G4LogicalVolume(
        solid_SourceShield,
        Pb,
        "logic_SourceShield"
    ); 

    G4double z_SourceShield = -Lab_Height/2 
                              + Floor_Height 
                              + ProfileLong_Height
                              + WheelEff_Height 
                              + SourceShield_Height/2;
    auto SourceShield_Vec = G4ThreeVector(0., 0., z_SourceShield);
    phys_SourceShield = new G4PVPlacement(
        0,
        SourceShield_Vec,
        logic_SourceShield,
        "phys_SourceShield",
        logic_Lab,
        false,
        0,
        true
    );

    /* VisAttributes */
    auto SourceShield_Colour = G4Colour(1., 0., 0.);
    G4VisAttributes *vis_SourceShield = new G4VisAttributes(SourceShield_Colour);
    vis_SourceShield  ->SetVisibility(true);
    vis_SourceShield  ->SetForceSolid(true);
    logic_SourceShield->SetVisAttributes(vis_SourceShield);

}

void MyDetectorConstruction::ConstructPLYBlocks(){

    G4double PLYBlock_Width  = m_hGeoParams["PLYBlock_Width"];
    G4double PLYBlock_Length = m_hGeoParams["PLYBlock_Length"];
    G4double PLYBlock_Height = m_hGeoParams["PLYBlock_Height"];

    G4double PLYWheel_Height    = m_hGeoParams["PLYWheel_Height"];
    G4double ProfileLong_Height = m_hGeoParams["ProfileLong_Height"];
    G4double Floor_Height       = m_hGeoParams["Floor_Height"];
    G4double Lab_Height         = m_hGeoParams["Lab_Height"];

    G4double SourceShield_oRadius = m_hGeoParams["SourceShield_oRadius"];

    G4Box* solid_PLYBlock = new G4Box(
        "solid_PLYBlock",
        PLYBlock_Width/2,
        PLYBlock_Length/2,
        PLYBlock_Height/2
    );

    logic_PLYBlock = new G4LogicalVolume(
        solid_PLYBlock,
        Polyethylene,
        "logic_PLYBlock"
    );

    G4double z_PLYBlock = -Lab_Height/2 
                          + Floor_Height 
                          + ProfileLong_Height 
                          + PLYWheel_Height 
                          + PLYBlock_Height/2;

    G4double d_ShieldBlock = 0.1*cm;

    // Place Block 0
    G4double x_PLYBlock0 = SourceShield_oRadius + PLYBlock_Width/2 + d_ShieldBlock;
    G4double y_PLYBlock0 = 0.*cm;
    auto PLYBlock0_Vec = G4ThreeVector(x_PLYBlock0, y_PLYBlock0, z_PLYBlock);
    phys_PLYBlock0 = new G4PVPlacement(
        0,
        PLYBlock0_Vec,
        logic_PLYBlock,
        "phys_PLYBlock0",
        logic_Lab,
        false,
        0,
        true
    );

    // Place Block 1
    G4double x_PLYBlock1 = -SourceShield_oRadius - PLYBlock_Width/2 - d_ShieldBlock;
    G4double y_PLYBlock1 = 0.*cm;
    auto PLYBlock1_Vec = G4ThreeVector(x_PLYBlock1, y_PLYBlock1, z_PLYBlock);
    phys_PLYBlock1 = new G4PVPlacement(
        0,
        PLYBlock1_Vec,
        logic_PLYBlock,
        "phys_PLYBlock1",
        logic_Lab,
        false,
        1,
        true
    ); 

    /* VisAttributes */
    auto PLYBlock_Colour = G4Colour(1., 0.95, 0.95);
    G4VisAttributes *vis_PLYBlock = new G4VisAttributes(PLYBlock_Colour);
    vis_PLYBlock  ->SetVisibility(true);
    vis_PLYBlock  ->SetForceSolid(true);
    vis_PLYBlock  ->SetForceAuxEdgeVisible(true); 
    logic_PLYBlock->SetVisAttributes(vis_PLYBlock);

}

void MyDetectorConstruction::ConstructCrystals() {

    G4double Crystal_Width  = m_hGeoParams["Crystal_Width"];
    G4double Crystal_Length = m_hGeoParams["Crystal_Length"];
    G4double Crystal_Height = m_hGeoParams["Crystal_Height"];

    G4double CrystalSource_Distance = m_hGeoParams["CrystalSource_Distance"];

    G4double PLYWheel_Height    = m_hGeoParams["PLYWheel_Height"];
    G4double Wheel_Height       = m_hGeoParams["Wheel_Height"];
    G4double Paper_Height       = m_hGeoParams["Paper_Height"];
    G4double ProfileLong_Height = m_hGeoParams["ProfileLong_Height"];
    G4double Floor_Height       = m_hGeoParams["Floor_Height"];
    G4double Lab_Height         = m_hGeoParams["Lab_Height"];

    G4double WheelEff_Height;
    if (usePLYWheel) {
        WheelEff_Height = PLYWheel_Height;
    } else {
        WheelEff_Height = Wheel_Height + Paper_Height;
    }

    
    G4Box* solid_Crystal = new G4Box(
        "solid_Crystal",
        Crystal_Width/2,
        Crystal_Length/2,
        Crystal_Height/2
    );

    logic_Crystal = new G4LogicalVolume(
        solid_Crystal,
        LiF,
        "logic_Crystal"
    );

    G4double z_Crystal = -Lab_Height/2
                        + Floor_Height 
                        + ProfileLong_Height 
                        + WheelEff_Height 
                        + Crystal_Height/2;
    auto Crystal0_Vec = G4ThreeVector(CrystalSource_Distance, 0., z_Crystal);
    phys_Crystal0 = new G4PVPlacement(
        0,
        Crystal0_Vec,
        logic_Crystal,
        "phys_Crystal0",
        logic_Lab,
        false,
        0,
        true
    );

    auto Crystal1_Vec = G4ThreeVector(-CrystalSource_Distance, 0., z_Crystal);
    phys_Crystal1 = new G4PVPlacement(
        0,
        Crystal1_Vec,
        logic_Crystal,
        "phys_Crystal1",
        logic_Lab,
        false,
        1,
        true
    );

    auto Crystal2_Vec = G4ThreeVector(0., CrystalSource_Distance, z_Crystal);
    phys_Crystal2 = new G4PVPlacement(
        0,
        Crystal2_Vec,
        logic_Crystal,
        "phys_Crystal2",
        logic_Lab,
        false,
        2,
        true
    );

    auto Crystal3_Vec = G4ThreeVector(0., -CrystalSource_Distance, z_Crystal);
    phys_Crystal3 = new G4PVPlacement(
        0,
        Crystal3_Vec,
        logic_Crystal,
        "phys_Crystal3",
        logic_Lab,
        false,
        3,
        true
    );
        
    /* VisAttributes*/
    auto Crystal_Colour = G4Colour(1., 1., 0.);
    G4VisAttributes *vis_Crystal = new G4VisAttributes(Crystal_Colour);
    vis_Crystal   ->SetVisibility(true);
    vis_Crystal   ->SetForceSolid(true);
    vis_Crystal   ->SetForceAuxEdgeVisible(true); 
    logic_Crystal->SetVisAttributes(vis_Crystal);

}


void MyDetectorConstruction::ConstructSDandField(){

    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    logic_Crystal->SetSensitiveDetector(sensDet);

}



