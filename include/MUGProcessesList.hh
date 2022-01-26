#ifndef _MUG_PROCESSES_LIST_HH_
#define _MUG_PROCESSES_LIST_HH_

#include <map>
#include <memory>

#include "G4VModularPhysicsList.hh"
#include "G4GenericMessenger.hh"
#include "globals.hh"

class MUGProcessesMessenger;
class MUGProcessesList : public G4VModularPhysicsList {

  public:

    MUGProcessesList();

    MUGProcessesList           (MUGProcessesList const&) = delete;
    MUGProcessesList& operator=(MUGProcessesList const&) = delete;
    MUGProcessesList           (MUGProcessesList&&)      = delete;
    MUGProcessesList& operator=(MUGProcessesList&&)      = delete;

    void SetCuts() override;

  protected:

    // virtual void ConstructParticle() override;
    // virtual void ConstructProcess() override;

  private:

    std::unique_ptr<G4GenericMessenger> fMessenger = nullptr;
    void DefineCommands();
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
