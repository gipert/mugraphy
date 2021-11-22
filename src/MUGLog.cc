/*
 * Copyright (C) 2007-2018, the BAT core developer team
 * All rights reserved.
 *
 * For the licensing terms see doc/COPYING.
 * For documentation see http://mpp.mpg.de/bat
 */

// ---------------------------------------------------------

#include "MUGLog.hh"

#include "ProjectInfo.hh"

#if MUG_HAS_ROOT
#include <TError.h>
#include <TROOT.h>
#endif

#include <iomanip>
#include <unistd.h> // for isatty()
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <memory>
#include <algorithm>

std::ofstream MUGLog::fOutputFileStream;

MUGLog::LogLevel MUGLog::fMinimumLogLevelFile = MUGLog::debug;

MUGLog::LogLevel MUGLog::fMinimumLogLevelScreen = MUGLog::summary;

bool MUGLog::fFirstOutputDone = false;

bool MUGLog::fUsePrefix = true;

std::string MUGLog::fVersion = MUG_PROJECT_VERSION;

// initialize them at start of program - mandatory
// so that even if user redirects, we've got a copy
std::streambuf const *coutbuf = G4cout.rdbuf();
std::streambuf const *cerrbuf = G4cerr.rdbuf();

// ---------------------------------------------------------

MUGLog::MUGLog() {

#if MUG_HAS_ROOT
    // suppress the ROOT Info printouts
    gErrorIgnoreLevel = 2000;
#endif
}

// ---------------------------------------------------------

void MUGLog::OpenLogFile(const std::string& filename) {

#if MUG_HAS_ROOT
    // suppress the ROOT Info printouts
    gErrorIgnoreLevel = 2000;
#endif

    // first close and flush and existing log file
    CloseLog();

    // open log file
    MUGLog::fOutputFileStream.open(filename.data());

    if (!MUGLog::fOutputFileStream.is_open()) {
        G4cerr << " Could not open log file " << filename << ". " << G4endl;
        return;
    }

    MUGLog::Out(MUGLog::summary, MUGLog::summary, "Opening logfile " + filename);
}

// ---------------------------------------------------------

void MUGLog::StartupInfo() {

    std::string message = "";
    message += "  _ __ ___ _ __ ___   __ _  __ _  ___ \n";
    message += " | '__/ _ \\ '_ ` _ \\ / _` |/ _` |/ _ \\\n";
    message += " | | |  __/ | | | | | (_| | (_| |  __/\n";
    message += " |_|  \\___|_| |_| |_|\\__,_|\\__, |\\___| v" + MUGLog::fVersion + "\n";
    message += "                           |___/      \n";

    // write message to screen
    if (MUGLog::fMinimumLogLevelScreen < MUGLog::nothing)
        G4cout << message << G4endl;

    if (MUGLog::IsOpen() && MUGLog::fMinimumLogLevelFile < MUGLog::nothing)
        MUGLog::fOutputFileStream << message;

    fFirstOutputDone = true;
}

// ---------------------------------------------------------

std::string MUGLog::GetPrefix(MUGLog::LogLevel loglevel, std::ostream& os) {

    if (!fUsePrefix) return "";

    switch (loglevel) {
        case debug:
            return Colorize<MUGLog::Ansi::magenta>("[Debug ---> ", os);
        case detail:
            return Colorize<MUGLog::Ansi::blue>   ("[Detail --> ", os);
        case summary:
            return Colorize<MUGLog::Ansi::green>  ("[Summary -> ", os);
        case warning:
            return Colorize<MUGLog::Ansi::yellow> ("[Warning -> ", os);
        case error:
            return Colorize<MUGLog::Ansi::red>    ("[Error ---> ", os);
        case fatal:
            return Colorize<MUGLog::Ansi::red>    ("[Fatal ---> ", os, true);
        default:
            return "";
    }
}

// https://github.com/agauniyal/rang/blob/master/include/rang.hpp
bool MUGLog::SupportsColors(const std::ostream& os) {

  // determine whether the stream refers to a file or a screen
  auto osbuf = os.rdbuf();
  FILE* the_stream = nullptr;
  if (osbuf == coutbuf) {
    the_stream = stdout;
  }
  else if (osbuf == cerrbuf) {
    the_stream = stderr;
  }
  else return false;

  // check that we are on a tty
  if (!::isatty(::fileno(the_stream))) return false;

  // check the value of the TERM variable
  const char* terms[] = {
    "ansi", "color", "console", "cygwin", "gnome",
    "konsole", "kterm", "linux", "msys", "putty",
    "rxvt", "screen", "vt100", "xterm"
  };

  auto env_p = std::getenv("TERM");
  if (env_p == nullptr) return false;

  return std::any_of(std::begin(terms), std::end(terms),
    [&](const char *term) { return ::strstr(env_p, term) != nullptr; });
}

// vim: tabstop=2 shiftwidth=2 expandtab
