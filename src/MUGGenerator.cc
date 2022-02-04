#include "MUGGenerator.hh"

#include "math.h"

#include "G4GenericMessenger.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleMomentum.hh"
#include "G4ParticleTypes.hh"
#include "Randomize.hh"

#include "ProjectInfo.hh"
#include "MUGLog.hh"
#include "MUGManager.hh"
#include "MUGDetectorConstruction.hh"
#include "MUGTools.hh"

#include "magic_enum/magic_enum.hpp"
#include "EcoMug/EcoMug.h"

namespace u = CLHEP;

MUGGenerator::MUGGenerator() {
  this->DefineCommands();

  // docs: https://doi.org/10.1016/j.nima.2021.165732
  fEcoMug = std::make_unique<EcoMug>();
  fGun = std::make_unique<G4ParticleGun>();
}

void MUGGenerator::BeginOfRunAction() {

  // NOTE: must match the GDML geometry
  auto sky_height = 170 * u::m;
  auto world_side = 330 * u::m;
  auto ground_depth = 50 * u::m;

  if (sky_height < world_side/2) {
    MUGLog::Out(MUGLog::fatal, "Sky is not high enough, it does not contain the full muon generation region");
  }

  MUGLog::Out(MUGLog::debug, "Configuring EcoMug");

  switch (fSkyShape) {
    case SkyShape::kPlane: {
      fEcoMug->SetUseSky();
      // put sky exactly on the top of the world
      fEcoMug->SetSkyCenterPosition({0, 0, (sky_height + ground_depth)/2 / u::m});
      auto sky_size = fSkyPlaneSize > 0 ? fSkyPlaneSize : world_side;
      fEcoMug->SetSkySize({sky_size/u::m, sky_size/u::m});
      break;
    }
    case SkyShape::kSphere: {
      fEcoMug->SetUseHSphere();
      // now lay the sphere exactly on the ground
      fEcoMug->SetHSphereCenterPosition({0, 0, (ground_depth - (sky_height+ground_depth)/2) / u::m});
      fEcoMug->SetHSphereRadius(world_side/2 / u::m); // no units, the user can decide. I use meters
      break;
    }
    default : {
      MUGLog::OutFormat(MUGLog::fatal, "\"{}\" sky shape not implemented!",
          magic_enum::enum_name<SkyShape>(fSkyShape));
      break;
    }
  }

  fEcoMug->SetMinimumMomentum(fMomentumMin / u::GeV);
  fEcoMug->SetMaximumMomentum(fMomentumMax / u::GeV);
  fEcoMug->SetMinimumTheta(fThetaMin / u::rad);
  fEcoMug->SetMaximumTheta(fThetaMax / u::rad);
  fEcoMug->SetMinimumPhi(fPhiMin / u::rad);
  fEcoMug->SetMaximumPhi(fPhiMax / u::rad);

  // FIXME: somehow this always sets the same seed
  // MUGLog::OutFormat(MUGLog::debug, "EcoMug random seed: {}", CLHEP::HepRandom::getTheSeed());
  // fEcoMug->SetSeed(CLHEP::HepRandom::getTheSeed());
}

void MUGGenerator::GeneratePrimaries(G4Event* event) {

  fEcoMug->Generate();

  MUGLog::OutFormat(MUGLog::debug, "Generated µ has charge {:+}", fEcoMug->GetCharge());
  fGun->SetNumberOfParticles(1);
  fEcoMug->GetCharge() < 0 ?
    fGun->SetParticleDefinition(G4MuonMinus::Definition()) :
    fGun->SetParticleDefinition(G4MuonPlus::Definition());

  const auto& pos = fEcoMug->GetGenerationPosition(); // no units, the user can decide. I use meters
  MUGLog::OutFormat(MUGLog::debug, "...origin ({:.4g}, {:.4g}, {:.4g}) m", pos[0], pos[1], pos[2]);
  fGun->SetParticlePosition({pos[0]*u::m, pos[1]*u::m, pos[2]*u::m});

  G4ThreeVector d_cart(1, 1, 1);
  d_cart.setTheta(fEcoMug->GetGenerationTheta()); // in rad
  d_cart.setPhi(fEcoMug->GetGenerationPhi()); // in rad
  d_cart.setMag(1*u::m);
  fGun->SetParticleMomentumDirection(d_cart);

  MUGLog::OutFormat(MUGLog::debug, "...direction (θ,φ) = ({:.4g}, {:.4g}) rad",
      fEcoMug->GetGenerationTheta(), fEcoMug->GetGenerationPhi());
  MUGLog::OutFormat(MUGLog::debug, "...direction (x,y,z) = ({:.4g}, {:.4g}, {:.4g}) m",
      d_cart.getX()/u::m, d_cart.getY()/u::m, d_cart.getZ()/u::m);

  const auto& p_tot = fEcoMug->GetGenerationMomentum() * u::GeV;
  MUGLog::OutFormat(MUGLog::debug, "...momentum {:.4g} GeV/c", p_tot/u::GeV);
  const auto& mu_mass = G4MuonPlus::Definition()->GetPDGMass();
  fGun->SetParticleEnergy(std::sqrt(p_tot*p_tot + mu_mass*mu_mass) - mu_mass);

  fGun->GeneratePrimaryVertex(event);
}

void MUGGenerator::SetSkyShape(std::string shape) {
  try { fSkyShape = MUGTools::ToEnum<MUGGenerator::SkyShape>(shape, "sky shape"); }
  catch (const std::bad_cast&) { return; }
}

void MUGGenerator::DefineCommands() {

  // NOTE: SetUnit(Category) is not thread-safe

  fMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/Generator/",
      "Commands for controlling the µ generator");

  fMessenger->DeclareMethod("SkyShape", &MUGGenerator::SetSkyShape)
    .SetGuidance("Geometrical shape of the µ generation surface")
    .SetParameterName("shape", false)
    .SetCandidates(MUGTools::GetCandidates<MUGGenerator::SkyShape>())
    .SetToBeBroadcasted(true)
    .SetStates(G4State_PreInit, G4State_Idle);

  fMessenger->DeclarePropertyWithUnit("SkyPlaneSize", "m", fSkyPlaneSize)
    .SetGuidance("Length of the side of the sky, if it has a planar shape")
    .SetParameterName("l", false)
    .SetRange("l > 0")
    .SetToBeBroadcasted(true)
    .SetStates(G4State_PreInit, G4State_Idle);

  fMessenger->DeclarePropertyWithUnit("MomentumMin", "GeV/c", fMomentumMin)
    .SetGuidance("Minimum momentum of the generated muon")
    .SetParameterName("p", false)
    .SetRange("p >= 0 && p < 1000")
    .SetToBeBroadcasted(true)
    .SetStates(G4State_PreInit, G4State_Idle);

  fMessenger->DeclarePropertyWithUnit("MomentumMax", "GeV/c", fMomentumMax)
    .SetGuidance("Maximum momentum of the generated muon")
    .SetParameterName("p", false)
    .SetRange("p > 0 && a <= 1000")
    .SetToBeBroadcasted(true)
    .SetStates(G4State_PreInit, G4State_Idle);

  fMessenger->DeclarePropertyWithUnit("ThetaMin", "deg", fThetaMin)
    .SetGuidance("Minimum zenith angle of the generated muon")
    .SetParameterName("a", false)
    .SetRange("a >= 0 && a < 90")
    .SetToBeBroadcasted(true)
    .SetStates(G4State_PreInit, G4State_Idle);

  fMessenger->DeclarePropertyWithUnit("ThetaMax", "deg", fThetaMax)
    .SetGuidance("Maximum zenith angle of the generated muon")
    .SetParameterName("a", false)
    .SetRange("a > 0 && a <= 90")
    .SetToBeBroadcasted(true)
    .SetStates(G4State_PreInit, G4State_Idle);

  fMessenger->DeclarePropertyWithUnit("PhiMin", "deg", fPhiMin)
    .SetGuidance("Minimum polar angle of the generated muon")
    .SetParameterName("a", false)
    .SetRange("a >= 0 && a < 360")
    .SetToBeBroadcasted(true)
    .SetStates(G4State_PreInit, G4State_Idle);

  fMessenger->DeclarePropertyWithUnit("PhiMax", "deg", fPhiMax)
    .SetGuidance("Maximum polar angle of the generated muon")
    .SetParameterName("a", false)
    .SetRange("a > 0 && a <= 360")
    .SetToBeBroadcasted(true)
    .SetStates(G4State_PreInit, G4State_Idle);
}

// vim: tabstop=2 shiftwidth=2 expandtab
