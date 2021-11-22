#include "MUGTrackingAction.hh"

#include "MUGEventAction.hh"
#include "G4Track.hh"

MUGTrackingAction::MUGTrackingAction(MUGEventAction* eventaction) :
  fEventAction(eventaction) {}

void MUGTrackingAction::PreUserTrackingAction(const G4Track* /*aTrack*/) {}

void MUGTrackingAction::PostUserTrackingAction(const G4Track* /*aTrack*/) {}

// vim: tabstop=2 shiftwidth=2 expandtab
