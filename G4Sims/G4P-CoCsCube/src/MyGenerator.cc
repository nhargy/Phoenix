#include "MyGenerator.hh"
#include "G4IonTable.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
  fParticleGun = new G4ParticleGun(1);

  fMessengerSource = new G4GenericMessenger(this,
                                            "/MySource/",
                                            "MySource");

  fMessengerSource->DeclareProperty("SourceHeight",
                                    SourceHeight,
                                    "Height of source from PLA holder");

  SourceHeight = 0.5;
}


PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  G4IonTable *ionTable = G4IonTable::GetIonTable();

  // Define Co-60 (Z=27, A=60, Excitation Energy = 0)
  G4ParticleDefinition *cobalt60 = ionTable->GetIon(27, 60, 0.0);

  // Define Cs-137
  // G4ParticleDefinition *cesium137 = ionTable->GetIon(55, 137, 0.0);

  if (!cobalt60) {
    G4cerr << "Error: Cobalt-60 ion not found in ion table!" << G4endl;
    return;
  }

  //fParticleGun->SetParticleDefinition(cobalt60);
  fParticleGun->SetParticleDefinition(cobalt60);
  fParticleGun->SetParticleCharge(0); // Ensure neutral Co-60 atom

  // **Enable radioactive decay for this particle**
  cobalt60->SetPDGLifeTime(0.0); // Allow Geant4 to handle decay
  //cesium137->SetPDGLifeTime(0.0); // Allow Geant4 to handle decay

  // Set initial position
  G4ThreeVector position(0, 0, -20*cm + SourceHeight*cm);
  fParticleGun->SetParticlePosition(position);

  // Set momentum direction (stationary source)
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 0.));

  // Set kinetic energy to zero so it remains at rest and undergoes decay
  fParticleGun->SetParticleEnergy(0.0 * MeV);

  // Generate the primary vertex
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
