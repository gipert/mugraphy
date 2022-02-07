#ifndef _MUG_MANAGEMENT_STEPPING_ACTION_HH_
#define _MUG_MANAGEMENT_STEPPING_ACTION_HH_

#include <memory>
#include <limits>

#include "G4UserSteppingAction.hh"
#include "CLHEP/Units/SystemOfUnits.h"

class G4Step;
class MUGEventAction;
class G4GenericMessenger;
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

    std::unique_ptr<G4GenericMessenger> fMessenger = nullptr;
    void DefineCommands();

    MUGEventAction* fEventAction = nullptr;
    float fLowestAltitudeForTracking = std::numeric_limits<float>::lowest() * CLHEP::m;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
