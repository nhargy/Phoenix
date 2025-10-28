#include "MyPrimaryGenerator.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4RandomTools.hh"

// PrimaryGeneratorAction.cc (snippet)
#include "SpectrumSampler.hh"
#include "Randomize.hh"  // G4UniformRand

// Build once (constructor or first call). Paste your full table:
// Values taken from Pavel's sim
static SpectrumSampler makeAmBeSampler() {
  std::vector<double> E = {
    0.0, 0.000000414, 0.11, 0.33, 0.54, 0.75, 0.97, 1.18, 1.40, 1.61,
    1.82, 2.04, 2.25, 2.47, 2.68, 2.90, 3.11, 3.32, 3.54, 3.75,
    3.97, 4.18, 4.39, 4.61, 4.82, 5.04, 5.25, 5.47, 5.68, 5.89,
    6.11, 6.32, 6.54, 6.75, 6.96, 7.18, 7.39, 7.61, 7.82, 8.03,
    8.25, 8.46, 8.68, 8.89, 9.11, 9.32, 9.53, 9.75, 9.96, 10.18,
    10.39, 10.60, 10.82, 11.03, 11.09
  };
  std::vector<double> W = {
    0.0, 0.01440, 0.03340, 0.03130, 0.02810, 0.02500, 0.02140, 0.01980, 0.01750, 0.01920,
    0.02230, 0.02150, 0.02250, 0.02280, 0.02950, 0.03560, 0.03690, 0.03460, 0.03070, 0.03000,
    0.02690, 0.02860, 0.03180, 0.03070, 0.03330, 0.03040, 0.02740, 0.02330, 0.02060, 0.01820,
    0.01770, 0.02040, 0.01830, 0.01630, 0.01680, 0.01680, 0.01880, 0.01840, 0.01690, 0.01440,
    0.00963, 0.00652, 0.00426, 0.00367, 0.00381, 0.00506, 0.00625, 0.00552, 0.00468, 0.00370,
    0.00278, 0.00151, 0.00036, 0.00000, 0.00000
  };
  return SpectrumSampler(std::move(E), std::move(W));
}

MyPrimaryGenerator::MyPrimaryGenerator(){

    // Define neutron
    fNeutronGun = new G4ParticleGun(1);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* neutron = particleTable->FindParticle("neutron");
    fNeutronGun->SetParticleDefinition(neutron);

    G4ThreeVector pos(0.,0.,21.5*cm);
    fNeutronGun->SetParticlePosition(pos);

    // Define gamma
    fGammaGun = new G4ParticleGun(1);

    G4ParticleDefinition* gamma = particleTable->FindParticle("gamma");
    fGammaGun->SetParticleDefinition(gamma);

    fGammaGun->SetParticlePosition(pos);

    // Open CSV file and write header
    fOut.open("generated_neutrons.csv");
    fOut << "event_id,neutron_energy_MeV\n";


}

MyPrimaryGenerator::~MyPrimaryGenerator(){
    if (fOut.is_open()) fOut.close();
    delete fNeutronGun;
    delete fGammaGun;  
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* event){

    static SpectrumSampler sampler = makeAmBeSampler();

    const double u = G4UniformRand();
    const double ENeutron = sampler.sample(u);
    // Save energy to CSV
    fOut << ENeutron << "\n";

    // Gamma energy
    G4double EGamma = 4.44 *MeV;

    // Random isotropic direction
    G4ThreeVector vecNeutron = G4RandomDirection();
    G4ThreeVector vecGamma = G4RandomDirection();

    fNeutronGun->SetParticleMomentumDirection(vecNeutron);
    fNeutronGun->SetParticleEnergy(ENeutron *MeV);
    fNeutronGun->GeneratePrimaryVertex(event);
   
    if(G4UniformRand() < 0.6){
        fGammaGun->SetParticleMomentumDirection(vecGamma);
        fGammaGun->SetParticleEnergy(EGamma);
        fGammaGun->GeneratePrimaryVertex(event);
    }
}
