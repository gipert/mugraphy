#ifndef _MUG_NAVIGATION_TOOLS_HH_
#define _MUG_NAVIGATION_TOOLS_HH_

#include "G4VPhysicalVolume.hh"

namespace MUGNavigationTools {

  G4VPhysicalVolume* FindVolumeByName(std::string name);
  G4VPhysicalVolume* FindDirectMother(G4VPhysicalVolume* volume);
  void PrintListOfLogicalVolumes();
  void PrintListOfPhysicalVolumes();
}

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
