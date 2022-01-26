#include "MUGDetectorConstruction.hh"

#include <filesystem>
namespace fs = std::filesystem;

#include "G4GDMLParser.hh"
#include "G4UserLimits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4VisAttributes.hh"
#include "CLHEP/Units/SystemOfUnits.h"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Para.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4GenericTrap.hh"
#include "G4Sphere.hh"
#include "G4Orb.hh"
#include "G4Polycone.hh"
#include "G4GenericPolycone.hh"
#include "G4Tet.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "MUGMaterialTable.hh"
#include "MUGLog.hh"
#include "MUGNavigationTools.hh"
#include "MUGPanelSD.hh"
#include "G4SDManager.hh"

namespace u = CLHEP;

MUGDetectorConstruction::MUGDetectorConstruction() {
  fMaterialTable = std::make_unique<MUGMaterialTable>();

  this->DefineCommands();
}

G4VPhysicalVolume* MUGDetectorConstruction::DefineGeometry() {
  return nullptr;
}

G4VPhysicalVolume* MUGDetectorConstruction::Construct() {

  MUGLog::Out(MUGLog::debug, "Constructing detector");

  if (!fGDMLFiles.empty()) {
    MUGLog::Out(MUGLog::debug, "Setting up G4GDMLParser");
    G4GDMLParser parser;
    parser.SetOverlapCheck(true);
    for (const auto& file : fGDMLFiles) {
      MUGLog::Out(MUGLog::detail, "Reading ", file, " GDML file");
      if (!fs::exists(fs::path(file.data()))) MUGLog::Out(MUGLog::fatal, file, " does not exist");
      parser.Read(file, false);
    }
    fWorld = parser.GetWorldVolume();

    // TODO: does this make sense?
    this->DefineGeometry();
  }
  else {
    fWorld = this->DefineGeometry();
    if (!fWorld) MUGLog::Out(MUGLog::fatal, "DefineGeometry() returned nullptr. ",
        "Did you forget to reimplement the base class method?");
  }

  for (const auto& el : fPhysVolStepLimits) {
    MUGLog::OutFormat(MUGLog::debug, "Setting max user step size for volume '{}' to {}", el.first, el.second);
    auto vol = MUGNavigationTools::FindVolumeByName(el.first);
    if (!vol) {
      MUGLog::Out(MUGLog::error, "Returned volume is null, skipping user step limit setting");
    }
    else vol->GetLogicalVolume()->SetUserLimits(new G4UserLimits(el.second));
  }

  // TODO: decide what to do
  // if (fWorld) fWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::GetInvisible());
  auto att = new G4VisAttributes(G4Colour::Cyan());
  att->SetForceWireframe(true);
  if (fWorld) fWorld->GetLogicalVolume()->SetVisAttributes(att);

  return fWorld;
}

void MUGDetectorConstruction::ConstructSDandField() {

  auto sd_man = G4SDManager::GetSDMpointer();
  if (MUGLog::GetLogLevelScreen() <= MUGLog::debug) sd_man->SetVerboseLevel(1);
  else sd_man->SetVerboseLevel(0);

  auto sd = new MUGPanelSD("Detector", "Panels");
  sd_man->AddNewDetector(sd);
  this->SetSensitiveDetector("Detector", sd);
}

void MUGDetectorConstruction::DefineCommands() {

  fMessenger = std::make_unique<G4GenericMessenger>(this, "/MUG/Geometry/",
      "Commands for controlling geometry definitions");

  fMessenger->DeclareMethod("IncludeGDMLFile", &MUGDetectorConstruction::IncludeGDMLFile)
    .SetGuidance("Use GDML file for geometry definition")
    .SetParameterName("filename", false)
    .SetStates(G4State_PreInit);

  fMessenger->DeclareMethod("PrintListOfLogicalVolumes", &MUGDetectorConstruction::PrintListOfLogicalVolumes)
    .SetGuidance("Print list of defined physical volumes")
    .SetStates(G4State_Idle);

  fMessenger->DeclareMethod("PrintListOfPhysicalVolumes", &MUGDetectorConstruction::PrintListOfPhysicalVolumes)
    .SetGuidance("Print list of defined physical volumes")
    .SetStates(G4State_Idle);
}

// vim: tabstop=2 shiftwidth=2 expandtab
