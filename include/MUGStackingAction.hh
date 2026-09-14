#ifndef _MUG_MANAGEMENT_STACKING_ACTION_HH_
#define _MUG_MANAGEMENT_STACKING_ACTION_HH_

#include <memory>

#include "G4UserStackingAction.hh"

#include "MUGLog.hh"

class G4Track;
class MUGEventAction;
class G4GenericMessenger;
class MUGStackingAction : public G4UserStackingAction {

  public:

    MUGStackingAction(MUGEventAction*);
    ~MUGStackingAction() = default;

    MUGStackingAction(MUGStackingAction const&) = delete;
    MUGStackingAction& operator=(MUGStackingAction const&) = delete;
    MUGStackingAction(MUGStackingAction&&) = delete;
    MUGStackingAction& operator=(MUGStackingAction&&) = delete;

    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) override;
    void NewStage() override;
    void PrepareNewEvent() override;

    void BeginOfRunAction();
    inline void EndOfRunAction() {};

    void KillSecondaries(bool flag = true) { fKillSecondaries = flag; }

  private:

    MUGEventAction* fEventAction = nullptr;

    std::unique_ptr<G4GenericMessenger> fMessenger = nullptr;
    void DefineCommands();

    bool fKillSecondaries = true;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
