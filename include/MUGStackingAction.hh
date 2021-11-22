#ifndef _MUG_MANAGEMENT_STACKING_ACTION_HH_
#define _MUG_MANAGEMENT_STACKING_ACTION_HH_

#include "G4UserStackingAction.hh"

class G4Track;
class MUGEventAction;
class MUGStackingAction : public G4UserStackingAction {

  public:

    MUGStackingAction(MUGEventAction*);
    ~MUGStackingAction() = default;

    MUGStackingAction           (MUGStackingAction const&) = delete;
    MUGStackingAction& operator=(MUGStackingAction const&) = delete;
    MUGStackingAction           (MUGStackingAction&&)      = delete;
    MUGStackingAction& operator=(MUGStackingAction&&)      = delete;

    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) override;
    void NewStage() override;
    void PrepareNewEvent() override;

  private:

    MUGEventAction* fEventAction;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
