#include "MUGPhysics.hh"

#include "G4ParticleTypes.hh"
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

MUGPhysics::MUGPhysics() :
  G4VModularPhysicsList() {

  G4VModularPhysicsList::verboseLevel = MUGLog::GetLogLevelScreen() <= MUGLog::debug ? 1 : 0;
  this->SetVerboseLevel(G4VModularPhysicsList::verboseLevel);

  G4MuonMinus::Definition();
  G4MuonPlus::Definition();

  this->SetCuts();

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

  // Activate producton of fission fragments in neutronHP
  G4ParticleHPManager::GetInstance()->SetProduceFissionFragments(true);

  // Stopping Physics
  this->RegisterPhysics(new G4StoppingPhysics(verbose));

  // Ion Physics
  this->RegisterPhysics(new G4IonElasticPhysics(verbose));
  this->RegisterPhysics(new G4IonQMDPhysics(verbose));

  this->DefineCommands();
}

////////////////////////////////////////////////////////////////////////////////////////////

void MUGPhysics::SetCuts() {

  MUGLog::Out(MUGLog::debug, "Setting particle cut values");

  // default production thresholds for the world volume
  // this should be enough since there are no dimensions smaller than 1 m
  this->SetDefaultCutValue(1*u::m);

  // Set different cuts for the sensitive region
  auto region_name = "SensitiveRegion";

  if (G4RegionStore::GetInstance()) {
    if (G4RegionStore::GetInstance()->size() > 1) {
      auto region = G4RegionStore::GetInstance()->GetRegion(region_name, false);
      if (region) {
        MUGLog::OutFormat(MUGLog::debug, "Registering production cuts for region '{}'", region_name);
        auto cuts = region->GetProductionCuts();
        if (!cuts) cuts = new G4ProductionCuts;
        cuts->SetProductionCut(1*u::mm, "mu-");
        cuts->SetProductionCut(1*u::mm, "mu+");
        region->SetProductionCuts(cuts);
      }
      else MUGLog::OutFormat(MUGLog::warning, "Could not find region '{}' for production cuts settings",
          region_name);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////

void MUGPhysics::DefineCommands() {

  fMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/Processes/",
      "Commands for controlling physics processes");
}

// vim: shiftwidth=2 tabstop=2 expandtab
