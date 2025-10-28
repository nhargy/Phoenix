#ifndef MY_PRIMARY_GENERATOR_HH
#define MY_PRIMARY_GENERATOR_HH

#include <fstream>
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4RandomTools.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction{
    
    public:
        MyPrimaryGenerator();
        ~MyPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event*);

    private:
        G4ParticleGun *fNeutronGun;
        G4ParticleGun *fGammaGun;

        std::ofstream fOut;  // output CSV file
};

#endif
