#include "MUGSteppingAction.hh"

#include "G4Step.hh"

#include "MUGEventAction.hh"

MUGSteppingAction::MUGSteppingAction(MUGEventAction* eventaction):
  fEventAction(eventaction) {}

void MUGSteppingAction::UserSteppingAction(const G4Step* /*step*/) {}

// vim: tabstop=2 shiftwidth=2 expandtab
