#ifndef _MUG_PANEL_SD_HH_
#define _MUG_PANEL_SD_HH_

#include <memory>
#include <string>

#include "G4VSensitiveDetector.hh"

#include "MUGPanelHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class G4GenericMessenger;
class MUGPanelSD : public G4VSensitiveDetector {

  public:

    MUGPanelSD(const std::string& name, const std::string& hits_coll_name);
    ~MUGPanelSD() = default;

    MUGPanelSD           (MUGPanelSD const&) = delete;
    MUGPanelSD& operator=(MUGPanelSD const&) = delete;
    MUGPanelSD           (MUGPanelSD&&)      = delete;
    MUGPanelSD& operator=(MUGPanelSD&&)      = delete;

    void Initialize(G4HCofThisEvent* hit_coll) override;
    bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void EndOfEvent(G4HCofThisEvent* hit_coll) override;

  private:

    MUGPanelHitsCollection* fHitsCollection = nullptr;

    std::unique_ptr<G4GenericMessenger> fMessenger = nullptr;
    void DefineCommands();
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
