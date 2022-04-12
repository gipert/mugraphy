#ifndef _MUG_NAVIGATION_TOOLS_HH_
#define _MUG_NAVIGATION_TOOLS_HH_

#include <string>

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

// TODO: write function that locates points in global coordinates by using an
// auxiliary G4Navigator. The G4Navigator instance must be unique and its
// access thread-safe.

namespace MUGNavigationTools {

  G4LogicalVolume* FindLogicalVolume(std::string name);
  G4VPhysicalVolume* FindPhysicalVolume(std::string name);
  G4VPhysicalVolume* FindDirectMother(G4VPhysicalVolume* volume);

  void PrintListOfLogicalVolumes();
  void PrintListOfPhysicalVolumes();
} // namespace MUGNavigationTools

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
