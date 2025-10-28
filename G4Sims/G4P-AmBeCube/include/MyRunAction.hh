#ifndef MY_RUN_ACTION_HH
#define MY_RUN_ACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"

class MyRunAction : public G4UserRunAction{

    public:
        MyRunAction();
        ~MyRunAction();

        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);
        
        void SetOutputDirectory(const G4String& dir) { outputDirectory = dir; }
        G4String GetOutputDirectory() const { return outputDirectory; }

    private:
        G4String outputDirectory;
};

#endif
