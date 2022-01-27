#include "ProjectInfo.hh"
#include "MUGManager.hh"
#include "MUGDetectorConstruction.hh"
#include "MUGLog.hh"

#include "CLI11/CLI11.hpp"

int main(int argc, char** argv) {

    int verbosity = MUGLog::summary;
    std::string macro;
    int nthreads = 0;
    bool batch = false;
    bool persistency = false;

    CLI::App app{"mugraphy"};
    app.set_version_flag("--version", MUG_PROJECT_VERSION);
    app.add_flag("-q{1}, -v{-2}", verbosity, " (default: summary info)");
    app.add_flag("-p, --persistency", persistency, "");
    app.add_option("-t, --nthreads", nthreads, "number of CPU threads (default: max available)");
    auto macro_arg = app.add_option("macro", macro, "simulation macro file name");
    app.add_flag("-b, --batch", batch, "run in batch mode")->needs(macro_arg);

    CLI11_PARSE(app, argc, argv);

    // HACK: I have to do this +2 because CLI11 resets verbosity to 0
    MUGLog::SetLogLevel(MUGLog::LogLevel(verbosity+2));

    MUGManager manager(argc, argv);
    manager.EnablePersistency(persistency);
    manager.SetBatchMode(batch);
    manager.SetNThreads(nthreads);

    // manager.GetManagementDetectorConstruction()->IncludeGDMLFile("gdml/main.gdml");

    if (!macro.empty()) manager.IncludeMacroFile(macro);

    manager.Initialize();
    manager.Run();

    return 0;
}
