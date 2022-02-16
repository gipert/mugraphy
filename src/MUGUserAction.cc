#include "MUGUserAction.hh"

#include "MUGManager.hh"
#include "MUGGenerator.hh"
#include "MUGRunAction.hh"
#include "MUGEventAction.hh"
#include "MUGStackingAction.hh"
#include "MUGTrackingAction.hh"
#include "MUGSteppingAction.hh"

void MUGUserAction::BuildForMaster() const {

  // the master thread does not simulate anything
  this->SetUserAction(new MUGRunAction(MUGManager::GetMUGManager()->IsPersistencyEnabled()));
}

void MUGUserAction::Build() const {

  auto generator_primary = new MUGGenerator();
  auto run_action = new MUGRunAction(generator_primary, MUGManager::GetMUGManager()->IsPersistencyEnabled());
  auto event_action = new MUGEventAction(run_action);

  this->SetUserAction(generator_primary);
  this->SetUserAction(event_action);
  this->SetUserAction(run_action);
  this->SetUserAction(new MUGStackingAction(event_action));
  this->SetUserAction(new MUGSteppingAction(event_action));
  this->SetUserAction(new MUGTrackingAction(event_action));
}

// vim: tabstop=2 shiftwidth=2 expandtab
