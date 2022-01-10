#include "MUGDetectorConstruction.hh"

#include <filesystem>
namespace fs = std::filesystem;

#include "G4GDMLParser.hh"
#include "G4UserLimits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

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

    auto world_s = new G4Box("WorldAir", 50*u::m, 50*u::m, 50*u::m);

    auto world_l = new G4LogicalVolume(world_s,
            MUGMaterialTable::GetMaterial("Air"),
            "WorldAir");

    auto world_p = new G4PVPlacement(nullptr,
            G4ThreeVector(),
            world_l,
            "WorldAir",
            0, false, 0);

    auto pyramid_s = new G4GenericTrap("Pyramid",
        20*u::m,
        {{-20*u::m, -20*u::m}, {-20*u::m, 20*u::m}, {20*u::m, 20*u::m}, {20*u::m, -20*u::m},
         {0*u::m, 0*u::m}, {0*u::m, 0*u::m}, {0*u::m, 0*u::m}, {0*u::m, 0*u::m}});

    auto pyramid_l = new G4LogicalVolume(pyramid_s,
        MUGMaterialTable::GetMaterial("Concrete"),
        "Pyramid");

    new G4PVPlacement(nullptr,
        G4ThreeVector(),
        pyramid_l,
        "Pyramid",
        world_l,
        false, 0);

    auto det_s = new G4Box("Detector", 1*u::m, 1*u::m, 1*u::m);

    auto det_l = new G4LogicalVolume(det_s,
            MUGMaterialTable::GetMaterial("ScintPlastic"),
            "Detector");

    /* auto det_p = */ new G4PVPlacement(nullptr,
            G4ThreeVector(30*u::m, 0, 0),
            det_l,
            "Detector",
            world_l,
            false, 0);

    return world_p;
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
      parser.Read(file);
    }
    fWorld = parser.GetWorldVolume();
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

  return fWorld;
}

void MUGDetectorConstruction::ConstructSDandField() {

  if (!fSD.Get()) {
    fSD.Put(new MUGPanelSD("Detector", "PanelHitsCollection"));
    G4SDManager::GetSDMpointer()->AddNewDetector(fSD.Get());
    this->SetSensitiveDetector("Detector", fSD.Get());
  }
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
