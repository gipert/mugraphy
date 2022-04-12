#include "MUGDetectorConstruction.hh"

#include <filesystem>
namespace fs = std::filesystem;

#include "CLHEP/Units/SystemOfUnits.h"
#include "G4GDMLParser.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Region.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4UserLimits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4GenericPolycone.hh"
#include "G4GenericTrap.hh"
#include "G4IntersectionSolid.hh"
#include "G4Orb.hh"
#include "G4Para.hh"
#include "G4Polycone.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tet.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"

#include "G4SDManager.hh"
#include "MUGLog.hh"
#include "MUGMaterialTable.hh"
#include "MUGNavigationTools.hh"
#include "MUGPanelSD.hh"

namespace u = CLHEP;

MUGDetectorConstruction::MUGDetectorConstruction() {
  fMaterialTable = std::make_unique<MUGMaterialTable>();

  this->DefineCommands();
}

G4VPhysicalVolume* MUGDetectorConstruction::DefineGeometry() { return nullptr; }

std::string MUGDetectorConstruction::GetGDMLFilePath(const std::string& filename) {

  fs::path filepath(filename);
  fs::path sel_filepath; // where the search result will be stored

  MUGLog::OutFormat(MUGLog::detail, "Current working directory is '{}'", fs::current_path().string());

  // search the file in other possible locations (break at first search result)
  for (const auto& p : fGDMLSearchPaths) {
    try {
      // std::filesystem::canonical will except if file does not exist
      sel_filepath = fs::canonical(p / filepath);
      break;
    } catch (const std::exception& ex) {
      MUGLog::OutFormat(MUGLog::debug, "Tried '{}' but does not exist", (p / filepath).string());
    }
  }

  if (!fs::exists(sel_filepath)) {
    MUGLog::OutFormat(MUGLog::fatal, "could not find and existing path for file '{}'",
        filepath.string());
  }
  if (!fs::is_regular_file(sel_filepath)) {
    MUGLog::OutFormat(MUGLog::fatal, "best candidate file '{}' is not a regular file",
        sel_filepath.string());
  }

  MUGLog::OutFormat(MUGLog::debug, "Found '{}'", sel_filepath.string());
  return sel_filepath;
}

G4VPhysicalVolume* MUGDetectorConstruction::Construct() {

  MUGLog::Out(MUGLog::debug, "Constructing detector");

  if (!fGDMLFiles.empty()) {
    MUGLog::Out(MUGLog::debug, "Setting up G4GDMLParser");
    G4GDMLParser parser;
    parser.SetOverlapCheck(true);
    for (const auto& file : fGDMLFiles) {
      auto abs_file = this->GetGDMLFilePath(file);
      MUGLog::Out(MUGLog::detail, "Reading ", abs_file, " GDML file");
      parser.Read(abs_file, false);
    }
    fWorld = parser.GetWorldVolume();

    // TODO: does this make sense?
    this->DefineGeometry();
  } else {
    fWorld = this->DefineGeometry();
    if (!fWorld)
      MUGLog::Out(MUGLog::fatal, "DefineGeometry() returned nullptr. ",
          "Did you forget to reimplement the base class method?");
  }

  for (const auto& el : fPhysVolStepLimits) {
    MUGLog::OutFormat(MUGLog::debug, "Setting max user step size for volume '{}' to {}", el.first,
        el.second);
    auto vol = MUGNavigationTools::FindPhysicalVolume(el.first);
    if (!vol) {
      MUGLog::Out(MUGLog::error, "Returned volume is null, skipping user step limit setting");
    } else vol->GetLogicalVolume()->SetUserLimits(new G4UserLimits(el.second));
  }

  // create sensitive region for special production cuts
  auto det_lv = MUGNavigationTools::FindLogicalVolume("Detector");
  det_lv->SetRegion(fSensitiveRegion);
  fSensitiveRegion->AddRootLogicalVolume(det_lv);

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

  fMessenger
      ->DeclareMethod("PrintListOfLogicalVolumes", &MUGDetectorConstruction::PrintListOfLogicalVolumes)
      .SetGuidance("Print list of defined physical volumes")
      .SetStates(G4State_Idle);

  fMessenger
      ->DeclareMethod("PrintListOfPhysicalVolumes",
          &MUGDetectorConstruction::PrintListOfPhysicalVolumes)
      .SetGuidance("Print list of defined physical volumes")
      .SetStates(G4State_Idle);
}

// vim: tabstop=2 shiftwidth=2 expandtab
