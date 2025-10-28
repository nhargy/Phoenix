#ifndef MY_PHYSICS_HH
#define MY_PHYSICS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"

class MyPhysicsList : public G4VModularPhysicsList{
    public:
        MyPhysicsList();
        ~MyPhysicsList();
};

#endif
