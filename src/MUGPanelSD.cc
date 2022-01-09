#include "MUGPanelSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4GenericMessenger.hh"

MUGPanelSD::MUGPanelSD(const std::string& name, const std::string& hits_coll_name) :
  G4VSensitiveDetector(name) {

  G4VSensitiveDetector::collectionName.insert(hits_coll_name);

  this->DefineCommands();
}

void MUGPanelSD::Initialize(G4HCofThisEvent* /*hit_coll*/) {

  // fHitsCollection = new MUGPanelHitsCollection(
  //     G4VSensitiveDetector::SensitiveDetectorName,
  //     G4VSensitiveDetector::collectionName[0]);

  // ...
}

bool MUGPanelSD::ProcessHits(G4Step* /*step*/, G4TouchableHistory* /*history*/) {
  return false;
}

void MUGPanelSD::EndOfEvent(G4HCofThisEvent* /*hit_coll*/) {
  return;
}

void MUGPanelSD::DefineCommands() {

  fMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/SD/",
      "Commands for controlling stuff");
}

// vim: tabstop=2 shiftwidth=2 expandtab
