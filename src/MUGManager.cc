#include "MUGManager.hh"

#include <iostream>
#include <string>
#include <vector>
#include <random>

#include "G4Threading.hh"
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#endif
#include "G4RunManager.hh"
#include "G4RunManagerFactory.hh"
#include "G4VUserPhysicsList.hh"
#include "G4VisManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4GenericMessenger.hh"
#include "Randomize.hh"

#include "ProjectInfo.hh"
#include "MUGProcessesList.hh"
#include "MUGDetectorConstruction.hh"
#include "MUGUserAction.hh"
#include "MUGTools.hh"

#if MUG_HAS_ROOT
#include "TEnv.h"
#endif

MUGManager* MUGManager::fMUGManager = nullptr;

MUGManager::MUGManager(int argc, char** argv) :
  fArgc(argc),
  fArgv(argv),
  fIsRandControlled(false),
  fBatchMode(false),
  fPrintModulo(-1),
  fG4RunManager(nullptr),
  fG4VisManager(nullptr),
  fProcessesList(nullptr),
  fDetectorConstruction(nullptr),
  fUserAction(nullptr) {

  if (fMUGManager) MUGLog::Out(MUGLog::fatal, "MUGManager must be singleton!");
  fMUGManager = this;

#if MUG_HAS_ROOT
  gEnv->SetValue("Root.Stacktrace", 0);
#endif

  // FIXME: I don't like this here
  // this->SetUpDefaultG4RunManager();

  this->DefineCommands();
}

MUGManager::~MUGManager() {
  if (MUGLog::IsOpen()) MUGLog::CloseLog();
}

void MUGManager::Initialize() {

  MUGLog::Out(MUGLog::detail, "Initializing application");

  MUGLog::Out(MUGLog::debug, "Initializing default run manager");

  // Suppress the Geant4 header:
  // save underlying buffer and set null (only standard output)
  std::streambuf* orig_buf = std::cout.rdbuf();
  std::cout.rdbuf(nullptr);

  fG4RunManager = std::unique_ptr<G4RunManager>(
      G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default));
  fG4RunManager->SetVerboseLevel(0);

  // restore buffer
  std::cout.rdbuf(orig_buf);

  MUGLog::Out(MUGLog::debug, "Initializing default processes list");
  fProcessesList = new MUGProcessesList();

  MUGLog::Out(MUGLog::debug, "Initializing default visualization manager");
  fG4VisManager = std::make_unique<G4VisExecutive>("quiet");
  fG4VisManager->Initialize();

  std::string _str = "";
  for (const auto& i : fG4VisManager->GetAvailableGraphicsSystems()) {
    _str += i->GetNickname() + " ";
  }
  MUGLog::Out(MUGLog::detail, "Available graphic systems: ", _str);


  if (!fUserAction) {
    MUGLog::Out(MUGLog::debug, "Initializing default user action class");
    fUserAction = new MUGUserAction();
  }

  MUGLog::Out(MUGLog::debug, "Initializing default (empty) detector");
  fDetectorConstruction = new MUGDetectorConstruction();

  fG4RunManager->SetUserInitialization(fDetectorConstruction);
  fG4RunManager->SetUserInitialization(fProcessesList);
  fG4RunManager->SetUserInitialization(fUserAction);

  if (!fIsRandControlled) {
    std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
    std::random_device rd; // uses RDRND or /dev/urandom
    auto rand_seed = dist(rd);
    G4Random::setTheSeed(rand_seed);
    MUGLog::Out(MUGLog::summary, "CLHEP::HepRandom seed set to: ", rand_seed);
  }
}

void MUGManager::Run() {

  auto session = std::make_unique<G4UIExecutive>(fArgc, fArgv);
  session->SetPrompt(MUGLog::Colorize<MUGLog::Ansi::unspecified>("mugraphy> ", G4cout, true));

  auto UI = G4UImanager::GetUIpointer();
  for (const auto& macro : fMacroFileNames) {
    MUGLog::Out(MUGLog::summary, "Loading macro file: ", macro);
    UI->ApplyCommand("/control/execute " + macro);
  }

  if (!fBatchMode) {
    MUGLog::Out(MUGLog::summary, "Entering interactive mode");
    session->SessionStart();
  }
}

G4RunManager* MUGManager::GetG4RunManager() {
  return fG4RunManager.get();
}

G4VisManager* MUGManager::GetG4VisManager() {
  return fG4VisManager.get();
}

MUGDetectorConstruction* MUGManager::GetDetectorConstruction() {
  return fDetectorConstruction;
}

G4VUserPhysicsList* MUGManager::GetProcessesList() {
  return fProcessesList;
}

void MUGManager::SetLogLevelScreen(std::string level) {
  try { MUGLog::SetLogLevelScreen(MUGTools::ToEnum<MUGLog::LogLevel>(level, "logging level")); }
  catch (const std::bad_cast&) { return; }
}

void MUGManager::SetLogLevelFile(std::string level) {
  try { MUGLog::SetLogLevelFile(MUGTools::ToEnum<MUGLog::LogLevel>(level, "logging level")); }
  catch (const std::bad_cast&) { return; }
}

void MUGManager::SetRandEngine(std::string name) {
  if (name == "JamesRandom") {
    CLHEP::HepRandom::setTheEngine(new CLHEP::HepJamesRandom);
    MUGLog::Out(MUGLog::summary, "Using James random engine");
  }
  else if (name == "RanLux") {
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine);
    MUGLog::Out(MUGLog::summary, "Using RanLux random engine");
  }
  else if (name == "MTwist") {
    CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
    MUGLog::Out(MUGLog::summary, "Using MTwist random engine");
  }
  else {
    MUGLog::Out(MUGLog::error, "'", name, "' random engine unknown");
  }
}

void MUGManager::SetRandEngineSeed(long seed) {
  if (seed >= std::numeric_limits<long>::max()) {
    MUGLog::Out(MUGLog::error, "Seed ", seed, " is too large. Largest possible seed is ",
        std::numeric_limits<long>::max(), ". Setting seed to 0.");
    CLHEP::HepRandom::setTheSeed(0);
  }
  else CLHEP::HepRandom::setTheSeed(seed);
  MUGLog::Out(MUGLog::summary, "CLHEP::HepRandom seed set to: ", seed);

  fIsRandControlled = true;
}

void MUGManager::SetRandEngineInternalSeed(int index) {
  long seeds[2];
  int table_index = index/2;
  CLHEP::HepRandom::getTheTableSeeds(seeds, table_index);

  int array_index = index % 2;
  CLHEP::HepRandom::setTheSeed(seeds[array_index]);
  MUGLog::Out(MUGLog::summary, "CLHEP::HepRandom seed set to: ", seeds[array_index]);

  fIsRandControlled = true;
}

void MUGManager::SetRandSystemEntropySeed() {
  std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
  std::random_device rd; // uses RDRND or /dev/urandom
  auto rand_seed = dist(rd);
  CLHEP::HepRandom::setTheSeed(rand_seed);
  MUGLog::Out(MUGLog::summary, "CLHEP::HepRandom seed set to: ", rand_seed);

  fIsRandControlled = true;
}

void MUGManager::DefineCommands() {

  // TODO: uncomment ranges one it's fixed in G4

  fMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/Manager/",
      "General commands for controlling the application");

  fMessenger->DeclareMethod("Include", &MUGManager::IncludeMacroFile)
    .SetGuidance("Include macro file")
    .SetParameterName("filename", false)
    .SetStates(G4State_PreInit, G4State_Idle);

  fMessenger->DeclareMethod("PrintProgressModulo", &MUGManager::SetPrintModulo)
    .SetGuidance("How many processed events before progress information is displayed")
    .SetParameterName("n", false)
    // .SetRange("n > 0")
    .SetStates(G4State_PreInit, G4State_Idle);

  fLogMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/Manager/Logging/",
      "Commands for controlling application logging");

  fLogMessenger->DeclareMethod("LogLevelScreen", &MUGManager::SetLogLevelScreen)
    .SetGuidance("Set verbosity level on screen")
    .SetParameterName("level", false)
    .SetCandidates(MUGTools::GetCandidates<MUGLog::LogLevel>())
    .SetStates(G4State_PreInit, G4State_Idle);

  fLogMessenger->DeclareMethod("LogLevelFile", &MUGManager::SetLogLevelFile)
    .SetGuidance("Set verbosity level on file")
    .SetParameterName("level", false)
    .SetCandidates(MUGTools::GetCandidates<MUGLog::LogLevel>())
    .SetStates(G4State_PreInit, G4State_Idle);

  fLogMessenger->DeclareMethod("LogToFile", &MUGManager::SetLogToFileName)
    .SetGuidance("Set filename for dumping application output")
    .SetParameterName("filename", false)
    .SetStates(G4State_PreInit, G4State_Idle);

  fRandMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/Manager/Randomization/",
      "Commands for controlling randomization settings");

  fRandMessenger->DeclareMethod("RandomEngine", &MUGManager::SetRandEngine)
    .SetGuidance("Select the random engine (CLHEP)")
    .SetParameterName("name", false)
    .SetStates(G4State_PreInit, G4State_Idle);

  fRandMessenger->DeclareMethod("Seed", &MUGManager::SetRandEngineSeed)
    .SetGuidance("Select the initial seed for randomization (CLHEP::HepRandom::setTheSeed)")
    .SetParameterName("n", false)
    // .SetRange("n >= 0")
    .SetDefaultValue("1")
    .SetStates(G4State_PreInit, G4State_Idle);

  fRandMessenger->DeclareMethod("InternalSeed", &MUGManager::SetRandEngineInternalSeed)
    .SetGuidance("Select the initial seed for randomization by using the internal CLHEP table")
    .SetParameterName("index", false)
    // .SetRange("index >= 0 && index < 430")
    .SetStates(G4State_PreInit, G4State_Idle);

  fRandMessenger->DeclareMethod("UseSystemEntropy", &MUGManager::SetRandSystemEntropySeed)
    .SetGuidance("Select a random initial seed from system entropy")
    .SetStates(G4State_PreInit, G4State_Idle);
}

// vim: tabstop=2 shiftwidth=2 expandtab
