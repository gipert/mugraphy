#ifndef _MUG_MANAGEMENT_EVENT_ACTION_HH_
#define _MUG_MANAGEMENT_EVENT_ACTION_HH_

#include <memory>

#include "G4Event.hh"
#include "G4UserEventAction.hh"

class G4GenericMessenger;
class MUGRunAction;
class MUGVOutputManager;
class MUGEventAction : public G4UserEventAction {

  public:

    MUGEventAction(MUGRunAction*);
    inline ~MUGEventAction() = default;

    MUGEventAction(MUGEventAction const&) = delete;
    MUGEventAction& operator=(MUGEventAction const&) = delete;
    MUGEventAction(MUGEventAction&&) = delete;
    MUGEventAction& operator=(MUGEventAction&&) = delete;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

  private:

    std::unique_ptr<G4GenericMessenger> fMessenger;
    void DefineCommands();
    MUGRunAction* fRunAction = nullptr;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
