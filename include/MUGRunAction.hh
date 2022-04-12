#ifndef _MUG_MANAGEMENT_RUN_ACTION_HH_
#define _MUG_MANAGEMENT_RUN_ACTION_HH_

#include <chrono>
#include <memory>
#include <string>

#include "G4Accumulable.hh"
#include "G4UserRunAction.hh"

class G4Run;
class MUGRun;
class MUGGenerator;
class G4GenericMessenger;
class MUGRunAction : public G4UserRunAction {

  public:

    MUGRunAction(bool persistency = false);
    MUGRunAction(MUGGenerator*, bool persistency = false);
    ~MUGRunAction();

    MUGRunAction(MUGRunAction const&) = delete;
    MUGRunAction& operator=(MUGRunAction const&) = delete;
    MUGRunAction(MUGRunAction&&) = delete;
    MUGRunAction& operator=(MUGRunAction&&) = delete;

    void SetupAnalysisManager();
    G4Run* GenerateRun() override;
    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;
    inline void RegisterHit() { fTotalHits += 1; }

    // getters
    inline std::vector<int>& GetPanelNrVec() { return fPanelNr; }
    inline std::vector<float>& GetEdepVec() { return fEdep; }
    inline std::vector<float>& GetXHitVec() { return fXHit; }
    inline std::vector<float>& GetYHitVec() { return fYHit; }
    inline std::vector<float>& GetZHitVec() { return fZHit; }
    inline std::vector<float>& GetThetaVec() { return fTheta; }
    inline std::vector<float>& GetPhiVec() { return fPhi; }

  private:

    std::unique_ptr<G4GenericMessenger> fMessenger = nullptr;
    void DefineCommands();

    MUGRun* fMUGRun = nullptr;
    MUGGenerator* fMUGGenerator = nullptr;
    bool fIsPersistencyEnabled = false;
    G4Accumulable<int> fTotalHits = 0;

    // per sub-detector
    std::vector<int> fPanelNr;
    std::vector<float> fEdep;
    std::vector<float> fXHit;
    std::vector<float> fYHit;
    std::vector<float> fZHit;
    std::vector<float> fTheta;
    std::vector<float> fPhi;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
