#ifndef MY_ACTION_INITIALIZATION_HH
#define MY_ACTION_INITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "MyPrimaryGenerator.hh"
#include "MyRunAction.hh"

class MyActionInitialization : public G4VUserActionInitialization {
    public:
        MyActionInitialization(const G4String& outputPath = "./");
        ~MyActionInitialization();

        virtual void Build() const;

    private:
        G4String fOutputPath;
};

#endif
