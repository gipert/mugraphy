#ifndef _MUG_PROCESSES_MESSENGER_HH_
#define _MUG_PROCESSES_MESSENGER_HH_

#include <memory>

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "MUGUIcmdStepLimit.hh"

class MUGProcessesList;
class MUGProcessesMessenger : public G4UImessenger {

  public:

    MUGProcessesMessenger(MUGProcessesList*);
    ~MUGProcessesMessenger() = default;

    MUGProcessesMessenger           (MUGProcessesMessenger const&) = delete;
    MUGProcessesMessenger& operator=(MUGProcessesMessenger const&) = delete;
    MUGProcessesMessenger           (MUGProcessesMessenger&&)      = delete;
    MUGProcessesMessenger& operator=(MUGProcessesMessenger&&)      = delete;

    void SetNewValue(G4UIcommand*, G4String) override;

  private:

    MUGProcessesList* fProcessesList;
    std::unique_ptr<G4UIdirectory> fProcessesDir;
    std::unique_ptr<MUGUIcmdStepLimit>    fStepLimitCmd;
};

#endif

// vim: shiftwidth=2 tabstop=2 expandtab
