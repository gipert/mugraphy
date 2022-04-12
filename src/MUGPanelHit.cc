#include "MUGPanelHit.hh"

#include "CLHEP/Units/SystemOfUnits.h"
#include "G4UnitsTable.hh"

#include "MUGLog.hh"

namespace u = CLHEP;

G4ThreadLocal G4Allocator<MUGPanelHit>* MUGPanelHitAllocator = nullptr;

G4bool MUGPanelHit::operator==(const MUGPanelHit& right) const {
  return (this == &right) ? true : false;
}

void MUGPanelHit::Print() {
  // TODO: printing
  MUGLog::Out(MUGLog::debug, "Panel number: ", fPanelNr);
  MUGLog::Out(MUGLog::debug, "Deposited energy: ", G4BestUnit(fEdep, "Energy"));
  MUGLog::Out(MUGLog::debug, "HitPosition: (x,y,z) = ", fHitPos);
  MUGLog::Out(MUGLog::debug, "Direction: (θ,φ) = (", fMomDir.getTheta() / u::deg, ", ",
      fMomDir.getPhi() / u::deg, ") deg");
}

void MUGPanelHit::Add(int panel_nr, double edep, const G4ThreeVector& pos, const G4ThreeVector& dir) {
  fEdep += edep;
  if (!fIsInitialized) {
    fPanelNr = panel_nr;
    fHitPos = pos;
    fMomDir = dir;
    fIsInitialized = true;
  }
}

// vim: tabstop=2 shiftwidth=2 expandtab
