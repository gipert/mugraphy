#ifndef _MUG_MANAGEMENT_RUN_ACTION_HH_
#define _MUG_MANAGEMENT_RUN_ACTION_HH_

#include <chrono>
#include <memory>
#include <string>

#include "G4UserRunAction.hh"

class G4Run;
class MUGRun;
class MUGEventAction;
class MUGGenerator;
class G4GenericMessenger;
class MUGRunAction : public G4UserRunAction {

  public:

    MUGRunAction(MUGEventAction*, bool persistency=false);
    MUGRunAction(MUGEventAction*, MUGGenerator*, bool persistency = false);
    ~MUGRunAction();

    MUGRunAction           (MUGRunAction const&) = delete;
    MUGRunAction& operator=(MUGRunAction const&) = delete;
    MUGRunAction           (MUGRunAction&&)      = delete;
    MUGRunAction& operator=(MUGRunAction&&)      = delete;

    void SetupAnalysisManager();
    G4Run* GenerateRun() override;
    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

  private:

    std::unique_ptr<G4GenericMessenger> fMessenger = nullptr;
    void DefineCommands();

    MUGRun* fMUGRun = nullptr;
    MUGEventAction* fEventAction = nullptr;
    MUGGenerator* fMUGGenerator = nullptr;
    bool fIsPersistencyEnabled = false;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
