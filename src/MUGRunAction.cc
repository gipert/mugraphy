#include "MUGRunAction.hh"

#include <limits>
#include <ctime>
#include <cstdlib>

#include "G4GenericMessenger.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

#include "MUGRun.hh"
#include "MUGLog.hh"
#include "MUGManager.hh"
#include "MUGGenerator.hh"
#include "MUGEventAction.hh"

#include "fmt/chrono.h"

G4Run* MUGRunAction::GenerateRun() {
  fMUGRun = new MUGRun();
  return fMUGRun;
}

MUGRunAction::MUGRunAction(MUGEventAction* ev_action, bool persistency) :
  fEventAction(ev_action),
  fIsPersistencyEnabled(persistency) {

  this->DefineCommands();
  if (fIsPersistencyEnabled) { this->SetupAnalysisManager(); }
}

MUGRunAction::MUGRunAction(MUGEventAction* ev_action, MUGGenerator* gene, bool persistency) :
  fEventAction(ev_action),
  fMUGGenerator(gene),
  fIsPersistencyEnabled(persistency) {

  this->DefineCommands();
  if (fIsPersistencyEnabled) { this->SetupAnalysisManager(); }
}

MUGRunAction::~MUGRunAction() {
  // TODO: what about this?
  // G4AnalysisManager::Instance()->Clear();
}

void MUGRunAction::SetupAnalysisManager() {

  MUGLog::Out(MUGLog::debug, "Setting up analysis manager");

  auto ana_man = G4AnalysisManager::Instance();
  if (MUGLog::GetLogLevelScreen() <= MUGLog::debug) ana_man->SetVerboseLevel(10);
  else ana_man->SetVerboseLevel(0);

  if (!MUGManager::GetMUGManager()->IsExecSequential()) ana_man->SetNtupleMerging(true);

  if (!fEventAction) MUGLog::Out(MUGLog::fatal, "Event action pointer stored by MUGRunAction is null");

  // create tuples
  ana_man->CreateNtuple("ntuples", "Event information");
  ana_man->CreateNtupleFColumn("energy", fEventAction->GetEdepVec());
  ana_man->CreateNtupleFColumn("xhit",   fEventAction->GetXHitVec());
  ana_man->CreateNtupleFColumn("yhit",   fEventAction->GetYHitVec());
  ana_man->CreateNtupleFColumn("zhit",   fEventAction->GetZHitVec());
  ana_man->CreateNtupleFColumn("theta",  fEventAction->GetThetaVec());
  ana_man->CreateNtupleFColumn("phi",    fEventAction->GetPhiVec());
  ana_man->FinishNtuple();
}

void MUGRunAction::BeginOfRunAction(const G4Run*) {

  MUGLog::OutDev(MUGLog::debug, "Start of run action");

  auto manager = MUGManager::GetMUGManager();

  if (fIsPersistencyEnabled) {
    auto ana_man = G4AnalysisManager::Instance();
    // TODO: realpath
    if (this->IsMaster()) MUGLog::Out(MUGLog::summary, "Opening output file: ",
        manager->GetOutputFileName());
    ana_man->OpenFile(manager->GetOutputFileName());
  }

  if (fMUGGenerator) { fMUGGenerator->BeginOfRunAction(); }

  if (this->IsMaster()) {

    // TODO: can I call MUGStackingAction::BeginOfRunAction here?

    // save start time for future
    fMUGRun->SetStartTime(std::chrono::system_clock::now());
    auto tt = fmt::localtime(fMUGRun->GetStartTime());

    MUGLog::OutFormat(MUGLog::summary, "Starting run nr. {:d}. Current local time is {:%d-%m-%Y %H:%M:%S}",
        fMUGRun->GetRunID(), tt);
    MUGLog::OutFormat(MUGLog::summary, "Number of events to be processed: {:d}",
        fMUGRun->GetNumberOfEventToBeProcessed());
  }

  auto g4manager = G4RunManager::GetRunManager();
  auto tot_events = g4manager->GetNumberOfEventsToBeProcessed();
  if (manager->GetPrintModulo() <= 0 and tot_events >= 100) manager->SetPrintModulo(tot_events/10);
  else if (tot_events < 100) manager->SetPrintModulo(100);
}

void MUGRunAction::EndOfRunAction(const G4Run*) {

  MUGLog::OutDev(MUGLog::debug, "End of run action");

  if (fMUGGenerator) fMUGGenerator->EndOfRunAction();

  if (fIsPersistencyEnabled) {
    auto ana_man = G4AnalysisManager::Instance();
    ana_man->Write();
    ana_man->CloseFile();
  }

  if (this->IsMaster()) {
    auto time_now = std::chrono::system_clock::now();

    MUGLog::OutFormat(MUGLog::summary, "Run nr. {:d} completed. {:d} events simulated. Current local time is {:%d-%m-%Y %H:%M:%S}",
        fMUGRun->GetRunID(), fMUGRun->GetNumberOfEventToBeProcessed(), fmt::localtime(time_now));

    auto start_time = fMUGRun->GetStartTime();
    auto tot_elapsed_s = std::chrono::duration_cast<std::chrono::seconds>(time_now - start_time).count();
    long partial = 0;
    long elapsed_d = (tot_elapsed_s - partial) / 86400; partial += elapsed_d * 86400;
    long elapsed_h = (tot_elapsed_s - partial) / 3600;  partial += elapsed_h * 3600;
    long elapsed_m = (tot_elapsed_s - partial) / 60;    partial += elapsed_m * 60;
    long elapsed_s = tot_elapsed_s - partial;

    MUGLog::OutFormat(MUGLog::summary, "Stats: run time was {:d} days, {:d} hours, {:d} minutes and {:d} seconds",
        elapsed_d, elapsed_h, elapsed_m, elapsed_s);

    auto total_sec_hres = std::chrono::duration<double>(time_now - fMUGRun->GetStartTime()).count();

    double n_ev = fMUGRun->GetNumberOfEvent();
    MUGLog::OutFormat(MUGLog::summary, "Stats: average event processing time was {:.5g} seconds/event = {:.5g} events/second",
        total_sec_hres/n_ev, n_ev/total_sec_hres);

    if (n_ev < 100) MUGLog::Out(MUGLog::warning, "Event processing time might be inaccurate");
  }
}

void MUGRunAction::DefineCommands() {}

// vim: tabstop=2 shiftwidth=2 expandtab
