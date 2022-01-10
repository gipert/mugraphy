#include "MUGManager.hh"
#include "MUGDetectorConstruction.hh"
#include "MUGLog.hh"

int main(int argc, char** argv) {

    // MUGLog::SetLogLevel(MUGLog::debug);

    MUGManager manager(argc, argv);
    // manager.GetManagementDetectorConstruction()->IncludeGDMLFile("gdml/main.gdml");

    std::string macro = argc > 1 ? argv[1] : "";
    if (!macro.empty()) manager.IncludeMacroFile(macro);

    manager.Initialize();
    manager.Run();

    return 0;
}
