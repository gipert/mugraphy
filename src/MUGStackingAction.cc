#include "MUGStackingAction.hh"

#include "G4Track.hh"
#include "G4GenericMessenger.hh"

#include "MUGEventAction.hh"

MUGStackingAction::MUGStackingAction(MUGEventAction* eventaction) :
  fEventAction(eventaction) {

  this->DefineCommands();
}

G4ClassificationOfNewTrack MUGStackingAction::ClassifyNewTrack(const G4Track* track) {

  if (fKillSecondaries and track->GetParentID() > 0) return fKill;

  return fUrgent;
}

void MUGStackingAction::NewStage() {}

void MUGStackingAction::PrepareNewEvent() {}

void MUGStackingAction::BeginOfRunAction() {
  if (fKillSecondaries) MUGLog::Out(MUGLog::detail, "µ-induced secondary particles will *not* be tracked");
  else MUGLog::Out(MUGLog::detail, "µ-induced secondary particles will be tracked");
}

void MUGStackingAction::DefineCommands() {

  fMessenger = std::make_unique<G4GenericMessenger>(this,
      "/MUG/Stacking/", "Commands for controlling particle tracking");

  // FIXME: somehow, passing true/false to the command does not work
  fMessenger->DeclareMethod("KillSecondaries", &MUGStackingAction::KillSecondaries)
    .SetGuidance("Should the simulation track muon-produced secondary particles")
    .SetStates(G4State_PreInit, G4State_Idle)
    .SetToBeBroadcasted(true);
}
// vim: tabstop=2 shiftwidth=2 expandtab
