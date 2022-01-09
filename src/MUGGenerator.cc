#include "MUGGenerator.hh"

#include "math.h"

#include "G4GenericMessenger.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleGun.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "Randomize.hh"

#include "ProjectInfo.hh"
#include "MUGLog.hh"

#include "EcoMug/EcoMug.h"

namespace u = CLHEP;

MUGGenerator::MUGGenerator() {
  this->DefineCommands();

  fEcoMug = std::make_unique<EcoMug>();
  fGun = std::make_unique<G4ParticleGun>();

  // is this correct?
  fEcoMug->SetSeed(CLHEP::HepRandom::getTheSeed());
  // TODO: get sky height from real detector
  fEcoMug->SetUseHSphere();
  fEcoMug->SetHSphereRadius(40); // m
  fEcoMug->SetHSphereCenterPosition({0, 0, 0});
}

void MUGGenerator::GeneratePrimaries(G4Event* event) {

  fEcoMug->Generate();

  const auto& pos = fEcoMug->GetGenerationPosition();
  fGun->SetParticlePosition({pos[0]*u::m, pos[1]*u::m, pos[2]*u::m});

  G4ThreeVector d_cart;
  d_cart.setTheta(fEcoMug->GetGenerationTheta());
  d_cart.setPhi(fEcoMug->GetGenerationPhi());

  fGun->SetParticleMomentum(fEcoMug->GetGenerationMomentum()*u::MeV);
  fGun->SetParticleMomentumDirection(d_cart);

  // TODO: the muon needs to be defined somewhere first
  fGun->SetNumberOfParticles(1);
  fEcoMug->GetCharge() < 0 ?
    fGun->SetParticleDefinition(G4MuonMinus::Definition()) :
    fGun->SetParticleDefinition(G4MuonPlus::Definition());

  fGun->GeneratePrimaryVertex(event);
}

void MUGGenerator::DefineCommands() {

  fMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/Generator/",
      "Commands for controlling generators");
}

// vim: tabstop=2 shiftwidth=2 expandtab
