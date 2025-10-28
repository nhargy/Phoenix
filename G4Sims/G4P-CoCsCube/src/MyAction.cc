#include "MyAction.hh"
#include "MyGenerator.hh"
#include "MyTracking.hh"

MyActionInitialization::MyActionInitialization() : fRunAction(new MyRunAction()) {
}

MyActionInitialization::~MyActionInitialization(){
    delete fRunAction;
}

void MyActionInitialization::Build() const{
    SetUserAction(new PrimaryGeneratorAction);
    SetUserAction(fRunAction);
    SetUserAction(new MyTrackingAction());
};
