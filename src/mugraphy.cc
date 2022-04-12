#include "MUGDetectorConstruction.hh"
#include "MUGLog.hh"
#include "MUGManager.hh"
#include "ProjectInfo.hh"

#include "CLI11/CLI11.hpp"

int main(int argc, char** argv) {

  bool quiet = false, verbose = false, debug = false;
  std::string macro;
  int nthreads = 0;
  bool batch = false;
  bool persistency = false;

  CLI::App app{"mugraphy"};
  app.set_version_flag("--version", MUG_PROJECT_VERSION);
  app.add_flag("-v, --verbose", verbose, "increase verbosity");
  app.add_flag("-q, --quiet", quiet, "decrease verbosity");
  app.add_flag("--debug", debug, "maximum verbosity");
  app.add_flag("-p, --persistency", persistency, "");
  app.add_option("-t, --nthreads", nthreads, "number of CPU threads (default: max available)");
  auto macro_arg = app.add_option("macro", macro, "simulation macro file name");
  app.add_flag("-b, --batch", batch, "run in batch mode")->needs(macro_arg);

  CLI11_PARSE(app, argc, argv);

  MUGLog::SetLogLevel(MUGLog::summary);
  if (quiet) MUGLog::SetLogLevel(MUGLog::warning);
  if (verbose) MUGLog::SetLogLevel(MUGLog::detail);
  if (debug) MUGLog::SetLogLevel(MUGLog::debug);

  MUGManager manager(argc, argv);
  manager.EnablePersistency(persistency);
  manager.SetBatchMode(batch);
  manager.SetNThreads(nthreads);

  if (!macro.empty()) manager.IncludeMacroFile(macro);

  manager.Initialize();
  manager.Run();

  return 0;
}
