#include "MUGSteppingAction.hh"

#include "G4Step.hh"
#include "G4GenericMessenger.hh"

#include "MUGEventAction.hh"
#include "MUGManager.hh"
#include "MUGDetectorConstruction.hh"

MUGSteppingAction::MUGSteppingAction(MUGEventAction* eventaction):
  fEventAction(eventaction) {

    this->DefineCommands();
}

void MUGSteppingAction::UserSteppingAction(const G4Step* step) {
  if (step->GetPreStepPoint()->GetPosition().getZ() < fLowestAltitudeForTracking) {
    step->GetTrack()->SetTrackStatus(G4TrackStatus::fKillTrackAndSecondaries);
  }
}

void MUGSteppingAction::DefineCommands() {

  fMessenger = std::make_unique<G4GenericMessenger>(this,
      "/MUG/Stepping/", "Commands for controlling particle tracking");

  // TODO: this is not the altitude but the coordinate in the world
  fMessenger->DeclarePropertyWithUnit("KillTrackBelowAltitude", "m", MUGSteppingAction::fLowestAltitudeForTracking)
    .SetGuidance("Kill tracks below a certain altitude")
    .SetParameterName("a", false)
    .SetStates(G4State_PreInit, G4State_Idle)
    .SetToBeBroadcasted(true);
}

// vim: tabstop=2 shiftwidth=2 expandtab
