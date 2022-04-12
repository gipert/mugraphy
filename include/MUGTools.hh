#ifndef _MUG_TOOLS_HH_
#define _MUG_TOOLS_HH_

#include <chrono>
#include <ctime>
#include <memory>
#include <utility>
#include <vector>

namespace MUGTools {

  template<typename T> T ToEnum(std::string);

  template<typename T> std::string GetCandidates();
} // namespace MUGTools

#include "MUGTools.icc"

#endif

// vim: shiftwidth=2 tabstop=2 expandtab
