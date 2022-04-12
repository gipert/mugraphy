#ifndef _MUG_MANAGEMENT_USER_ACTION_HH_
#define _MUG_MANAGEMENT_USER_ACTION_HH_

#include "G4VUserActionInitialization.hh"

class MUGUserAction : public G4VUserActionInitialization {

  public:

    inline MUGUserAction() = default;
    ~MUGUserAction() override = default;

    MUGUserAction(MUGUserAction const&) = delete;
    MUGUserAction& operator=(MUGUserAction const&) = delete;
    MUGUserAction(MUGUserAction&&) = delete;
    MUGUserAction& operator=(MUGUserAction&&) = delete;

    void Build() const override;
    void BuildForMaster() const override;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
