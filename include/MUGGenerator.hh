#ifndef _MUG_GENERATOR_PRIMARY_HH_
#define _MUG_GENERATOR_PRIMARY_HH_

#include <memory>
#include <string>

#include "CLHEP/Units/SystemOfUnits.h"
#include "G4GenericMessenger.hh"
#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "EcoMug/EcoMug.h"

namespace u = CLHEP;

class MUGGenerator : public G4VUserPrimaryGeneratorAction {

  public:

    enum SkyShape {
      kPlane,
      kSphere
    };

    MUGGenerator();
    ~MUGGenerator() = default;

    MUGGenerator(MUGGenerator const&) = delete;
    MUGGenerator& operator=(MUGGenerator const&) = delete;
    MUGGenerator(MUGGenerator&&) = delete;
    MUGGenerator& operator=(MUGGenerator&&) = delete;

    void GeneratePrimaries(G4Event* event) override;
    void BeginOfRunAction();
    inline void EndOfRunAction() {}

  private:

    std::unique_ptr<EcoMug> fEcoMug = nullptr;
    std::unique_ptr<G4ParticleGun> fGun = nullptr;

    std::unique_ptr<G4GenericMessenger> fMessenger = nullptr;
    void DefineCommands();
    void SetSkyShape(std::string shape);

    SkyShape fSkyShape = kSphere;
    float fSkyPlaneSize = -1;

    float fSpherePositionThetaMin = 0 * u::deg;
    float fSpherePositionThetaMax = 90 * u::deg;
    float fSpherePositionPhiMin = 0 * u::deg;
    float fSpherePositionPhiMax = 360 * u::deg;

    float fMomentumMin = 0 * u::GeV;
    float fMomentumMax = 1 * u::TeV;
    float fThetaMin = 0 * u::deg;
    float fThetaMax = 90 * u::deg;
    float fPhiMin = 0 * u::deg;
    float fPhiMax = 360 * u::deg;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
