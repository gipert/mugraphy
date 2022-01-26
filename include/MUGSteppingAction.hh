#ifndef _MUG_MANAGEMENT_STEPPING_ACTION_HH_
#define _MUG_MANAGEMENT_STEPPING_ACTION_HH_

#include "G4UserSteppingAction.hh"

class G4Step;
class MUGEventAction;
class MUGSteppingAction : public G4UserSteppingAction {

  public:

    MUGSteppingAction(MUGEventAction*);
    ~MUGSteppingAction() = default;

    MUGSteppingAction           (MUGSteppingAction const&) = delete;
    MUGSteppingAction& operator=(MUGSteppingAction const&) = delete;
    MUGSteppingAction           (MUGSteppingAction&&)      = delete;
    MUGSteppingAction& operator=(MUGSteppingAction&&)      = delete;

    void UserSteppingAction(const G4Step*) override;

  private:

    MUGEventAction* fEventAction = nullptr;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
