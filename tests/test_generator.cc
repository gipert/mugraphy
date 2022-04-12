#include "math.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include "TFile.h"
#include "TNtuple.h"

#include "EcoMug/EcoMug.h"

namespace u = CLHEP;

int main() {

  // units: m
  const auto sky_height = 170;
  const auto world_side = 330;
  const auto ground_depth = 50;

  EcoMug gene;

  gene.SetUseHSphere();
  gene.SetHSphereCenterPosition({0, 0, (ground_depth - (sky_height + ground_depth) / 2)});
  gene.SetHSphereRadius(world_side / 2);

  gene.SetMinimumMomentum(20);

  TFile fout("test_generator.root", "recreate");
  TNtuple ntuple("events", "Generated primary events", "x:y:z:theta:phi:ekin");

  for (int i = 0; i < 5e5; ++i) {

    gene.Generate();

    const auto& pos = gene.GetGenerationPosition();

    const auto& p_tot = gene.GetGenerationMomentum() * u::GeV;
    const auto mu_mass = 105.658 * u::MeV;
    const auto ekin = std::sqrt(p_tot * p_tot + mu_mass * mu_mass) - mu_mass;

    ntuple.Fill(pos[0], pos[1], pos[2], // m
        gene.GetGenerationTheta(),      // rad
        gene.GetGenerationPhi(),        // rad
        ekin / u::GeV);                 // GeV
  }

  ntuple.Write();
  fout.Close();

  return 0;
}

// vim: tabstop=2 shiftwidth=2 expandtab
