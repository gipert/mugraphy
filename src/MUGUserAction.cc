#include "MUGUserAction.hh"

#include "MUGGenerator.hh"
#include "MUGRunAction.hh"
#include "MUGEventAction.hh"
#include "MUGStackingAction.hh"
#include "MUGTrackingAction.hh"
#include "MUGSteppingAction.hh"

void MUGUserAction::BuildForMaster() const {

  auto event_action = new MUGEventAction();
  // the master thread does not simulate anything, no primary generator is needed
  this->SetUserAction(new MUGRunAction(event_action));
}

void MUGUserAction::Build() const {

  auto generator_primary = new MUGGenerator();
  auto event_action = new MUGEventAction();
  this->SetUserAction(generator_primary);
  this->SetUserAction(new MUGRunAction(event_action, generator_primary));
  this->SetUserAction(event_action);
  this->SetUserAction(new MUGStackingAction(event_action));
  this->SetUserAction(new MUGSteppingAction(event_action));
  this->SetUserAction(new MUGTrackingAction(event_action));
}

// vim: tabstop=2 shiftwidth=2 expandtab
