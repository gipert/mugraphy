#include "MUGDetectorConstruction.hh"

#include <filesystem>
namespace fs = std::filesystem;

#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4GDMLParser.hh"

#include "MUGMaterialTable.hh"
#include "MUGLog.hh"
#include "MUGNavigationTools.hh"

MUGDetectorConstruction::MUGDetectorConstruction() {
  fMaterialTable = std::make_unique<MUGMaterialTable>();

  this->DefineCommands();
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

  // TODO: build and return world volume?

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
  // TODO
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
