#include "MUGPanelHit.hh"

#include "G4GenericMessenger.hh"

G4ThreadLocal G4Allocator<MUGPanelHit>* MUGPanelHitAllocator = nullptr;

MUGPanelHit::MUGPanelHit() {
  this->DefineCommands();
}

G4bool MUGPanelHit::operator==(const MUGPanelHit& right) const {
  return ( this == &right ) ? true : false;
}

void MUGPanelHit::DefineCommands() {

  fMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/Hit/",
      "Commands for controlling stuff");
}

// vim: tabstop=2 shiftwidth=2 expandtab
