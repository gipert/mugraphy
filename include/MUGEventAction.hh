#ifndef _MUG_MANAGEMENT_EVENT_ACTION_HH_
#define _MUG_MANAGEMENT_EVENT_ACTION_HH_

#include <memory>

#include "G4Event.hh"
#include "G4UserEventAction.hh"

class G4GenericMessenger;
class MUGVOutputManager;
class MUGEventAction : public G4UserEventAction {

  public:

    MUGEventAction();
    inline ~MUGEventAction() = default;

    MUGEventAction           (MUGEventAction const&) = delete;
    MUGEventAction& operator=(MUGEventAction const&) = delete;
    MUGEventAction           (MUGEventAction&&)      = delete;
    MUGEventAction& operator=(MUGEventAction&&)      = delete;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    // getters
    inline std::vector<float>& GetEdepVec()  { return fEdep; }
    inline std::vector<float>& GetXHitVec()  { return fXHit; }
    inline std::vector<float>& GetYHitVec()  { return fYHit; }
    inline std::vector<float>& GetZHitVec()  { return fZHit; }
    inline std::vector<float>& GetThetaVec() { return fTheta; }
    inline std::vector<float>& GetPhiVec()   { return fPhi; }

  private:

    std::unique_ptr<G4GenericMessenger> fMessenger;
    void DefineCommands();

    int fHitCollID = -1;

    // per sub-detector
    std::vector<float> fEdep;
    std::vector<float> fXHit;
    std::vector<float> fYHit;
    std::vector<float> fZHit;
    std::vector<float> fTheta;
    std::vector<float> fPhi;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
