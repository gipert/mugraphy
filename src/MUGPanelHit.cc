#include "MUGPanelHit.hh"

#include "G4GenericMessenger.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4UnitsTable.hh"

#include "MUGLog.hh"

namespace u = CLHEP;

G4ThreadLocal G4Allocator<MUGPanelHit>* MUGPanelHitAllocator = nullptr;

MUGPanelHit::MUGPanelHit() {
  this->DefineCommands();
}

G4bool MUGPanelHit::operator==(const MUGPanelHit& right) const {
  return ( this == &right ) ? true : false;
}

void MUGPanelHit::Print() {
  // TODO: printing
  MUGLog::Out(MUGLog::debug, "Deposited energy: ", G4BestUnit(fEdep, "Energy"));
  MUGLog::Out(MUGLog::debug, "HitPosition: (x,y,z) = ", fHitPos);
  MUGLog::Out(MUGLog::debug, "Direction: (θ,φ) = (", fMomDir.getTheta() / u::deg,
      ", ", fMomDir.getPhi() / u::deg, ") deg");
}

void MUGPanelHit::Add(double edep, const G4ThreeVector& pos, const G4ThreeVector& dir) {
  fEdep += edep;
  if (!fIsInitialized) {
    fHitPos = pos;
    fMomDir = dir;
    fIsInitialized = true;
  }
}

void MUGPanelHit::DefineCommands() {

  fMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/Hit/",
      "Commands for controlling stuff");
}

// vim: tabstop=2 shiftwidth=2 expandtab
