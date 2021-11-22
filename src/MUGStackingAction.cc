#include "MUGStackingAction.hh"

#include "MUGEventAction.hh"

MUGStackingAction::MUGStackingAction(MUGEventAction* eventaction) :
  fEventAction(eventaction) {}

G4ClassificationOfNewTrack MUGStackingAction::ClassifyNewTrack(const G4Track* /*aTrack*/) {
  return fUrgent;
}

void MUGStackingAction::NewStage() {}

void MUGStackingAction::PrepareNewEvent() {}

// vim: tabstop=2 shiftwidth=2 expandtab
