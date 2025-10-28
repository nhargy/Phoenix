#include "MyActionInitialization.hh"

MyActionInitialization::MyActionInitialization(const G4String& outputPath) : fOutputPath(outputPath) {
}

MyActionInitialization::~MyActionInitialization() {
}

void MyActionInitialization::Build() const {
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    MyRunAction *runAction = new MyRunAction();
    runAction->SetOutputDirectory(fOutputPath);
    SetUserAction(runAction);

};
