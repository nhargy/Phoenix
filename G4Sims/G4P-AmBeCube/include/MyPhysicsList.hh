#ifndef MY_PHYSICS_LIST_HH
#define MY_PHYSICS_LIST_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh" // neutron recoil and capture processes
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4NeutronTrackingCut.hh"

class MyPhysicsList : public G4VModularPhysicsList{
    public:
        MyPhysicsList();
        ~MyPhysicsList();

};

#endif
