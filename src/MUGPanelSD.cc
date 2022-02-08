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
  const size_t panel_nr = step->GetPreStepPoint()->GetTouchable()->GetCopyNumber();

  MUGLog::Out(MUGLog::debug, "Hit in detector panel nr. ", panel_nr, " detected");

  MUGPanelHit* hit = nullptr;

  const auto& hit_vec = fHitsCollection->GetVector();
  const auto& result = std::find_if(hit_vec->begin(), hit_vec->end(),
      [&panel_nr](MUGPanelHit* h){ return h->GetPanelNr() == (int)panel_nr; });

  if (result == hit_vec->end()) {
    MUGLog::Out(MUGLog::debug, "No hit object found, initializing");
    hit = new MUGPanelHit();
    fHitsCollection->insert(hit);
  }
  else hit = *result;

  // integrate hit info
  MUGLog::Out(MUGLog::debug, "Adding hit data to panel hit container");
  hit->Add(
    panel_nr,
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
