#include "MUGMaterialTable.hh"

#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4MaterialPropertiesTable.hh"

#include "MUGLog.hh"
#include "MUGDetectorConstruction.hh"

namespace u = CLHEP;

std::map<std::string, std::string> MUGMaterialTable::fMaterialAliases = {};

MUGMaterialTable::MUGMaterialTable() {
  this->InitializeMaterials();
  this->DefineCommands();
}

G4Material* MUGMaterialTable::GetMaterial(std::string name) {

  auto man = G4NistManager::Instance();
  if (MUGLog::GetLogLevelScreen() <= MUGLog::debug) man->SetVerbose(2);

  if (fMaterialAliases.find(name) != fMaterialAliases.end()) {
    return man->FindOrBuildMaterial(fMaterialAliases[name]);
  }
  else return G4Material::GetMaterial("MUG_" + name);
}

void MUGMaterialTable::InitializeMaterials() {

  MUGLog::Out(MUGLog::detail, "Initializing materials");

  fMaterialAliases = {
    {"Vacuum",       "G4_Galactic"},
    {"Air",          "G4_AIR"},
    {"Concrete",     "G4_CONCRETE"},
    {"Water",        "G4_WATER"},
    {"ScintPlastic", "G4_PLASTIC_SC_VINYLTOLUENE"}
  };

  auto man = G4NistManager::Instance();
  if (MUGLog::GetLogLevelScreen() <= MUGLog::debug) man->SetVerbose(2);

  std::string name, symbol;
  std::vector<std::string> elements;
  std::vector<double> mass_fraction;
  double density;
  G4State state;
  double temperature;
  double pressure;
  double abundance;
  int n_isotopes;
  int n_components;
  int n_atoms;

  // define enriched germanium
  auto Ge70 = new G4Isotope(name="Ge70", 32, 70, 69.92 *u::g/u::mole);
  auto Ge72 = new G4Isotope(name="Ge72", 32, 72, 71.92 *u::g/u::mole);
  auto Ge73 = new G4Isotope(name="Ge73", 32, 73, 73.00 *u::g/u::mole);
  auto Ge74 = new G4Isotope(name="Ge74", 32, 74, 74.00 *u::g/u::mole);
  auto Ge76 = new G4Isotope(name="Ge76", 32, 76, 76.00 *u::g/u::mole);

  auto el_enr_ge = new G4Element(name="EnrichedGermanium", symbol="EnrGe", n_isotopes=5);
  el_enr_ge->AddIsotope(Ge70, abundance= 0.0 *u::perCent);
  el_enr_ge->AddIsotope(Ge72, abundance= 0.1 *u::perCent);
  el_enr_ge->AddIsotope(Ge73, abundance= 0.2 *u::perCent);
  el_enr_ge->AddIsotope(Ge74, abundance=13.1 *u::perCent);
  el_enr_ge->AddIsotope(Ge76, abundance=86.6 *u::perCent);

  auto mat_enr_ge = new G4Material("MUG_EnrichedGermanium",
      density      = 5.54,
      n_components = 1,
      state        = G4State::kStateSolid,
      temperature  = u::STP_Temperature,
      pressure     = u::STP_Pressure);
  mat_enr_ge->AddElement(el_enr_ge, n_atoms=1);

}

void MUGMaterialTable::DefineCommands() {

  fMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/Materials/",
      "Commands for controlling material definitions");
}

// vim: tabstop=2 shiftwidth=2 expandtab
