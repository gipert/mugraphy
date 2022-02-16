#include "MUGEventAction.hh"

#include <sstream>
#include <chrono>

#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"
#include "G4AnalysisManager.hh"
#include "G4SDManager.hh"

#include "MUGRun.hh"
#include "MUGPanelHit.hh"
#include "MUGManager.hh"
#include "MUGRunAction.hh"
#include "MUGUserAction.hh"
#include "MUGLog.hh"

#include "fmt/chrono.h"

MUGEventAction::MUGEventAction(MUGRunAction* run_action) :
  fRunAction(run_action) {
  this->DefineCommands();
}

void MUGEventAction::BeginOfEventAction(const G4Event* event) {

  auto print_modulo = MUGManager::GetMUGManager()->GetPrintModulo();
  if ((event->GetEventID()+1) % print_modulo == 0) {

    auto current_run = dynamic_cast<const MUGRun*>(G4RunManager::GetRunManager()->GetCurrentRun());
    auto tot_events = current_run->GetNumberOfEventToBeProcessed();

    // FIXME: elapsed time computation is broken in parallel mode

    auto start_time = current_run->GetStartTime();
    auto time_now = std::chrono::system_clock::now();
    auto tot_elapsed_s = std::chrono::duration_cast<std::chrono::seconds>(time_now - start_time).count();
    long partial = 0;
    long elapsed_d = (tot_elapsed_s - partial) / 86400; partial += elapsed_d * 86400;
    long elapsed_h = (tot_elapsed_s - partial) / 3600;  partial += elapsed_h * 3600;
    long elapsed_m = (tot_elapsed_s - partial) / 60;    partial += elapsed_m * 60;
    long elapsed_s = tot_elapsed_s - partial;

    // FIXME: how to cast the percentage to int?
    MUGLog::OutFormat(MUGLog::summary, "Processing event nr. {:d} ({:.0f}%), at {:d} days, {:d} hours, {:d} minutes and {} seconds",
        event->GetEventID()+1, (event->GetEventID()+1)*100./tot_events,
        elapsed_d, elapsed_h, elapsed_m, elapsed_s);
  }

  if (MUGManager::GetMUGManager()->IsPersistencyEnabled()) {
    fRunAction->GetPanelNrVec().clear();
    fRunAction->GetEdepVec().clear();
    fRunAction->GetXHitVec().clear();
    fRunAction->GetYHitVec().clear();
    fRunAction->GetZHitVec().clear();
    fRunAction->GetThetaVec().clear();
    fRunAction->GetPhiVec().clear();
  }
}

void MUGEventAction::EndOfEventAction(const G4Event* event) {

  auto sd_man = G4SDManager::GetSDMpointer();

  if (fHitCollID < 0) fHitCollID = sd_man->GetCollectionID("Detector/Panels");
  if (fHitCollID < 0) {
    MUGLog::Out(MUGLog::error, "Could not find sensitive detector Detector/Panels");
    return;
  }

  auto hit_coll = dynamic_cast<MUGPanelHitsCollection*>(event->GetHCofThisEvent()->GetHC(fHitCollID));
  if (!hit_coll) {
    MUGLog::Out(MUGLog::error, "Could not find hit collection associated with event");
    return;
  }

  if (hit_coll->entries() <= 0) {
    MUGLog::Out(MUGLog::debug, "Hit collection is empty at the end of event");
    return;
  }
  else {
    MUGLog::Out(MUGLog::debug, "Hit collection contains ", hit_coll->entries(),
      " panel hits at the end of the event");
    fRunAction->RegisterHit();
  }

  if (MUGManager::GetMUGManager()->IsPersistencyEnabled()) {
    MUGLog::Out(MUGLog::debug, "Filling data vectors for objects persistency");

    auto ana_man = G4AnalysisManager::Instance();

    for (auto hit : *hit_coll->GetVector()) {
      if (!hit) continue;
      hit->Print();
      fRunAction->GetPanelNrVec().push_back(hit->GetPanelNr());
      fRunAction->GetEdepVec().push_back(hit->GetEdep() / G4Analysis::GetUnitValue("MeV"));
      fRunAction->GetXHitVec().push_back(hit->GetHitPos().getX() / G4Analysis::GetUnitValue("m"));
      fRunAction->GetYHitVec().push_back(hit->GetHitPos().getY() / G4Analysis::GetUnitValue("m"));
      fRunAction->GetZHitVec().push_back(hit->GetHitPos().getZ() / G4Analysis::GetUnitValue("m"));
      fRunAction->GetThetaVec().push_back((CLHEP::pi - hit->GetMomDir().getTheta()) / G4Analysis::GetUnitValue("deg"));
      fRunAction->GetPhiVec().push_back(hit->GetMomDir().getPhi() / G4Analysis::GetUnitValue("deg"));
    }

    ana_man->AddNtupleRow();
  }
}

void MUGEventAction::DefineCommands() {}

// vim: tabstop=2 shiftwidth=2 expandtab
