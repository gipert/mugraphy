#ifndef _MUG_MANAGER_HH_
#define _MUG_MANAGER_HH_

#include <memory>
#include <vector>

#include "globals.hh"

#include "G4RunManager.hh"
#include "G4VisManager.hh"

#include "MUGLog.hh"

class G4VUserPhysicsList;
class MUGDetectorConstruction;
class MUGUserAction;
class MUGManagerMessenger;
class G4GenericMessenger;
class MUGManager {

  public:

    MUGManager() = delete;
    MUGManager(int argc, char** argv);
    ~MUGManager();

    MUGManager(MUGManager const&) = delete;
    MUGManager& operator=(MUGManager const&) = delete;
    MUGManager(MUGManager&&) = delete;
    MUGManager& operator=(MUGManager&&) = delete;

    // getters
    static inline MUGManager* GetMUGManager() { return fMUGManager; }
    G4RunManager* GetG4RunManager();
    G4VisManager* GetG4VisManager();
    MUGDetectorConstruction* GetDetectorConstruction();
    G4VUserPhysicsList* GetProcessesList();
    inline int GetPrintModulo() { return fPrintModulo; }

    inline bool IsExecSequential() {
      return fG4RunManager->GetRunManagerType() == G4RunManager::RMType::sequentialRM;
    }
    inline bool IsPersistencyEnabled() { return fIsPersistencyEnabled; }
    inline const std::string& GetOutputFileName() { return fOutputFile; }

    // setters
    inline void SetNThreads(int n_thr) { fNThreads = n_thr; }
    inline void SetBatchMode(bool flag = true) { fBatchMode = flag; }
    inline void SetPrintModulo(int n_ev) { fPrintModulo = n_ev > 0 ? n_ev : -1; }

    inline void EnablePersistency(bool flag = true) { fIsPersistencyEnabled = flag; }
    inline void IncludeMacroFile(std::string filename) { fMacroFileNames.emplace_back(filename); }
    void Initialize();
    void Run();

    void SetRandEngine(std::string name);
    void SetRandEngineSeed(long seed);
    void SetRandEngineInternalSeed(int index);
    void SetRandSystemEntropySeed();
    inline bool GetRandIsControlled() { return fIsRandControlled; }

    void SetLogLevelScreen(std::string level);
    void SetLogLevelFile(std::string level);
    inline void SetLogToFileName(std::string filename) { MUGLog::OpenLogFile(filename); }

  private:

    int fArgc;
    char** fArgv;
    std::vector<std::string> fMacroFileNames;
    bool fIsRandControlled = false;
    bool fBatchMode = false;
    bool fIsPersistencyEnabled = true;
    int fPrintModulo = -1;
    int fNThreads = 0;
    std::string fOutputFile = "detector-hits.root";

    static MUGManager* fMUGManager;

    std::unique_ptr<G4RunManager> fG4RunManager = nullptr;
    std::unique_ptr<G4VisManager> fG4VisManager = nullptr;

    G4VUserPhysicsList* fPhysicsList = nullptr;
    MUGDetectorConstruction* fDetectorConstruction = nullptr;
    MUGUserAction* fUserAction = nullptr;

    // messenger stuff
    std::unique_ptr<G4GenericMessenger> fMessenger = nullptr;
    std::unique_ptr<G4GenericMessenger> fLogMessenger = nullptr;
    std::unique_ptr<G4GenericMessenger> fRandMessenger = nullptr;
    std::unique_ptr<G4GenericMessenger> fOutputMessenger = nullptr;
    void DefineCommands();
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
