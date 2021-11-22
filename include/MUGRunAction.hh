#ifndef _MUG_MANAGEMENT_RUN_ACTION_HH_
#define _MUG_MANAGEMENT_RUN_ACTION_HH_

#include <chrono>

#include "globals.hh"
#include "G4UserRunAction.hh"

class G4Run;
class MUGRun;
class MUGGenerator;
class MUGRunAction : public G4UserRunAction {

  public:

    MUGRunAction() = default;
    MUGRunAction(MUGGenerator*);
    ~MUGRunAction() = default;

    MUGRunAction           (MUGRunAction const&) = delete;
    MUGRunAction& operator=(MUGRunAction const&) = delete;
    MUGRunAction           (MUGRunAction&&)      = delete;
    MUGRunAction& operator=(MUGRunAction&&)      = delete;

    G4Run* GenerateRun() override;
    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

  private:

    MUGRun* fMUGRun;
    MUGGenerator* fMUGGenerator;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
