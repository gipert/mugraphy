#include "MUGGenerator.hh"

#include "ProjectInfo.hh"
#include "MUGLog.hh"

#include "G4GenericMessenger.hh"
#include "G4ThreeVector.hh"

#include "MUGTools.hh"

MUGGenerator::MUGGenerator() {
  this->DefineCommands();
}

void MUGGenerator::GeneratePrimaries(G4Event* /*event*/) {}

void MUGGenerator::DefineCommands() {

  fMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/Generator/",
      "Commands for controlling generators");
}

// vim: tabstop=2 shiftwidth=2 expandtab
