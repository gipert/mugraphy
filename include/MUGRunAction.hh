#ifndef _MUG_MANAGEMENT_RUN_ACTION_HH_
#define _MUG_MANAGEMENT_RUN_ACTION_HH_

#include <chrono>

#include "globals.hh"
#include "G4UserRunAction.hh"

class G4Run;
class MUGRun;
class MUGEventAction;
class MUGGenerator;
class G4GenericMessenger;
class MUGRunAction : public G4UserRunAction {

  public:

    MUGRunAction(MUGEventAction*);
    MUGRunAction(MUGEventAction*, MUGGenerator*);
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
    std::string fOutputFile = "detector-hits.hdf5";
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
