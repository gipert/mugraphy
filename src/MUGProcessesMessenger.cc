#include "MUGProcessesMessenger.hh"

#include <map>
#include <exception>

#include "globals.hh"
#include "G4ProcessManager.hh"
#include "G4StepLimiter.hh"

#include "MUGProcessesList.hh"
#include "MUGManager.hh"
#include "MUGDetectorConstruction.hh"
#include "MUGTools.hh"
#include "MUGLog.hh"

MUGProcessesMessenger::MUGProcessesMessenger(MUGProcessesList* plist) :
  fProcessesList(plist) {

  G4String directory = "/MUG/Processes/";

  fProcessesDir = std::make_unique<G4UIdirectory>(directory);

  fStepLimitCmd = std::make_unique<MUGUIcmdStepLimit>(directory + "SetStepLimit", this);
}

void MUGProcessesMessenger::SetNewValue(G4UIcommand *cmd, G4String new_val) {

  if (cmd == fStepLimitCmd.get()) {
    std::string particle_name = fStepLimitCmd->GetParticleName(new_val);
    fProcessesList->LimitStepForParticle(particle_name);

    std::string volume_name = fStepLimitCmd->GetVolumeName(new_val);
    double max_step = fStepLimitCmd->GetStepSize(new_val);
    MUGManager::GetMUGManager()->GetDetectorConstruction()->SetMaxStepLimit(volume_name, max_step);
  }
}

// vim: shiftwidth=2 tabstop=2 expandtab
