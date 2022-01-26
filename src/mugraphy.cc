#include "MUGManager.hh"
#include "MUGDetectorConstruction.hh"
#include "MUGLog.hh"

#include "CLI11/CLI11.hpp"

int main(int argc, char** argv) {

    std::string macro;
    int nthreads = 0;

    CLI::App app{"mugraphy"};
    app.add_option("-t, --nthreads", nthreads, "number of CPU threads (default: max available)");
    app.add_option("macro", macro, "simulation macro file name");

    CLI11_PARSE(app, argc, argv);

    MUGLog::SetLogLevel(MUGLog::debug);

    MUGManager manager(argc, argv);
    // manager.GetManagementDetectorConstruction()->IncludeGDMLFile("gdml/main.gdml");
    manager.SetNThreads(nthreads);

    if (!macro.empty()) manager.IncludeMacroFile(macro);

    manager.Initialize();
    manager.Run();

    return 0;
}
