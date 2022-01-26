#include "MUGPanelSD.hh"

#include <map>

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4GenericMessenger.hh"

#include "MUGPanelHit.hh"
#include "MUGLog.hh"

MUGPanelSD::MUGPanelSD(const std::string& name, const std::string& hits_coll_name) :
  G4VSensitiveDetector(name) {

  G4VSensitiveDetector::collectionName.insert(hits_coll_name);

  this->DefineCommands();
}

void MUGPanelSD::Initialize(G4HCofThisEvent* hit_coll) {

  // create hits collection object
  fHitsCollection = new MUGPanelHitsCollection(
      G4VSensitiveDetector::SensitiveDetectorName,
      G4VSensitiveDetector::collectionName[0]);

  // associate it with the G4HofThisEvent object
  auto hc_id = G4SDManager::GetSDMpointer()->GetCollectionID(G4VSensitiveDetector::collectionName[0]);
  hit_coll->AddHitsCollection(hc_id, fHitsCollection);
}

bool MUGPanelSD::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/) {

  // which panel is this? Use copy number
  const size_t panel_nr = step->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(1); // mother
                                                                                  //
  MUGLog::Out(MUGLog::debug, "Hit in detector panel nr. ", panel_nr, " detected");

  // do we need to allocate space?
  if (panel_nr >= fHitsCollection->entries()) {
    MUGLog::Out(MUGLog::debug, "Hit container has size = ", fHitsCollection->entries(),
        ", padding with hit object until index = ", panel_nr);
    const auto n_missing = panel_nr - fHitsCollection->entries();
    for (size_t _ = 0; _ <= n_missing; _++) {
      fHitsCollection->insert(new MUGPanelHit());
    }
  }

  // get object
  auto hit = (*fHitsCollection)[panel_nr];

  // integrate hit info
  MUGLog::Out(MUGLog::debug, "Adding hit data to panel hit container");
  hit->Add(
    step->GetTotalEnergyDeposit(),
    step->GetPreStepPoint()->GetPosition(),
    step->GetTrack()->GetMomentumDirection()
  );

  return true;
}

void MUGPanelSD::EndOfEvent(G4HCofThisEvent* /*hit_coll*/) {
  return;
}

void MUGPanelSD::DefineCommands() {

  fMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/SD/",
      "Commands for controlling stuff");
}

// vim: tabstop=2 shiftwidth=2 expandtab
