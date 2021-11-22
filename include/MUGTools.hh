#ifndef _MUG_TOOLS_HH_
#define _MUG_TOOLS_HH_

#include <memory>
#include <vector>
#include <utility>
#include <ctime>
#include <chrono>

namespace MUGTools {

  template <typename T>
  T ToEnum(std::string);

  template <typename T>
  std::string GetCandidates();
}

#include "MUGTools.icc"

#endif

// vim: shiftwidth=2 tabstop=2 expandtab
