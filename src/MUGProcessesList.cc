#include "MUGProcessesList.hh"

#include "G4RegionStore.hh"
#include "G4HadronicParameters.hh"
#include "G4HadronicProcessStore.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsShielding.hh"
#include "G4ParticleHPManager.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonQMDPhysics.hh"

#include "MUGLog.hh"
#include "MUGTools.hh"

namespace u = CLHEP;

MUGProcessesList::MUGProcessesList() :
  G4VModularPhysicsList() {

  G4VModularPhysicsList::verboseLevel = 0;
  this->SetVerboseLevel(G4VModularPhysicsList::verboseLevel);

  const auto& verbose = G4VModularPhysicsList::verboseLevel;

  // G4VUserPhysicsList::AddTransportation();
  this->RegisterPhysics(new G4EmStandardPhysics(verbose));

  // Synchroton Radiation & GN Physics
  this->RegisterPhysics(new G4EmExtraPhysics(verbose));

  // Decays 
  this->RegisterPhysics(new G4DecayPhysics(verbose));
  this->RegisterPhysics(new G4RadioactiveDecayPhysics(verbose));

  // Hadron Elastic scattering
  this->RegisterPhysics(new G4HadronElasticPhysicsHP(verbose));

  this->RegisterPhysics(new G4HadronPhysicsShielding("hInelastic Shielding", verbose,
        G4HadronicParameters::Instance()->GetMinEnergyTransitionFTF_Cascade(),
        G4HadronicParameters::Instance()->GetMaxEnergyTransitionFTF_Cascade()));

  //Activate prodcuton of fission fragments in neutronHP
  G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( true );

  // Stopping Physics
  this->RegisterPhysics(new G4StoppingPhysics(verbose));

  // Ion Physics
  this->RegisterPhysics(new G4IonElasticPhysics(verbose));
  this->RegisterPhysics(new G4IonQMDPhysics(verbose));

  this->DefineCommands();
}

////////////////////////////////////////////////////////////////////////////////////////////

void MUGProcessesList::ConstructParticle() {

  MUGLog::Out(MUGLog::detail, "Constructing particles");

  return;
}

////////////////////////////////////////////////////////////////////////////////////////////

void MUGProcessesList::ConstructProcess() {

  return;
}

////////////////////////////////////////////////////////////////////////////////////////////

void MUGProcessesList::SetCuts() {

  MUGLog::Out(MUGLog::debug, "Setting particle cut values");

  G4HadronicProcessStore::Instance()->SetVerbose(G4VModularPhysicsList::verboseLevel);

  G4VUserPhysicsList::defaultCutValue = 1*u::mm;

  this->SetCutValue(0, "proton");
  this->SetCutValue(1*u::mm, "mu-");

  if (G4RegionStore::GetInstance()) {
    if (G4RegionStore::GetInstance()->size() > 1) {
      // Set different cuts for the sensitive region
      auto region = G4RegionStore::GetInstance()->GetRegion("SensitiveRegion", false);
      if (region) {
        MUGLog::Out(MUGLog::detail, "Register cuts for SensitiveRegion ");
        auto cuts = region->GetProductionCuts();
        if (!cuts) cuts = new G4ProductionCuts;
        cuts->SetProductionCut(1*u::mm, "mu-");
        region->SetProductionCuts(cuts);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////

void MUGProcessesList::DefineCommands() {

  fMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/Processes/",
      "Commands for controlling physics processes");
}

// vim: shiftwidth=2 tabstop=2 expandtab
