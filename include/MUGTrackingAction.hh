#ifndef _MUG_MANAGEMENT_TRACKING_ACTION_HH_
#define _MUG_MANAGEMENT_TRACKING_ACTION_HH_

#include "G4UserTrackingAction.hh"

class MUGEventAction;
class MUGTrackingAction : public G4UserTrackingAction {

  public:

    MUGTrackingAction(MUGEventAction*);
    ~MUGTrackingAction() = default;

    MUGTrackingAction           (MUGTrackingAction const&) = delete;
    MUGTrackingAction& operator=(MUGTrackingAction const&) = delete;
    MUGTrackingAction           (MUGTrackingAction&&)      = delete;
    MUGTrackingAction& operator=(MUGTrackingAction&&)      = delete;

    virtual void PreUserTrackingAction(const G4Track*) override;
    virtual void PostUserTrackingAction(const G4Track*) override;
    inline G4TrackingManager* GetTrackingManager() { return G4UserTrackingAction::fpTrackingManager; };

  private:

    MUGEventAction* fEventAction;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
