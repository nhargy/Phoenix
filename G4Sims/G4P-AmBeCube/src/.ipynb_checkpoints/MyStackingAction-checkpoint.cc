#include "MyStackingAction.hh"

#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

MyStackingAction::MyStackingAction() {
};

MyStackingAction::~MyStackingAction() {
};

G4ClassificationOfNewTrack MyStackingAction::ClassifyNewTrack(const G4Track* track) {
};

void MyStackingAction::NewStage() {
};

void MyStackingAction::PrepareNewEvent() {
};   
