#ifndef MY_RUN_HH
#define MY_RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include <string>

class MyRunAction : public G4UserRunAction{

    public:
        MyRunAction();
        ~MyRunAction();
        
        void SetOutputDirectory(const G4String& dir) { fOutputDirectory = dir; }
        
    private:
        G4String fOutputDirectory;

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

};

#endif
