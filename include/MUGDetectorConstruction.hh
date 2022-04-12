#ifndef _MUG_MANAGEMENT_DETECTOR_CONSTRUCTION_HH_
#define _MUG_MANAGEMENT_DETECTOR_CONSTRUCTION_HH_

#include <map>
#include <memory>
#include <vector>

#include "G4Cache.hh"
#include "G4Region.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "MUGMaterialTable.hh"
#include "MUGNavigationTools.hh"

class G4VPhysicalVolume;
class G4GenericMessenger;
class MUGPanelSD;
class MUGDetectorConstruction : public G4VUserDetectorConstruction {

  public:

    MUGDetectorConstruction();
    ~MUGDetectorConstruction() = default;

    MUGDetectorConstruction(MUGDetectorConstruction const&) = delete;
    MUGDetectorConstruction& operator=(MUGDetectorConstruction const&) = delete;
    MUGDetectorConstruction(MUGDetectorConstruction&&) = delete;
    MUGDetectorConstruction& operator=(MUGDetectorConstruction&&) = delete;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    G4VPhysicalVolume* DefineGeometry();
    inline void IncludeGDMLFile(std::string filename) { fGDMLFiles.emplace_back(filename); }
    inline void SetMaxStepLimit(std::string name, double max_step) {
      fPhysVolStepLimits.insert_or_assign(name, max_step);
    }
    inline void PrintListOfLogicalVolumes() { MUGNavigationTools::PrintListOfLogicalVolumes(); }
    inline void PrintListOfPhysicalVolumes() { MUGNavigationTools::PrintListOfPhysicalVolumes(); }

  private:

    std::string GetGDMLFilePath(const std::string& name);
    void DefineCommands();

    std::vector<std::string> fGDMLFiles;
    // order matters!
    std::vector<std::string> fGDMLSearchPaths = {".", // current directory
        "../share/mugraphy/gdml", // maybe we are using an installed version of mugraphy
        "../../../src/geom",      // or we have not installed it
        "../../src/geom", "../src/geom",
        "../mugraphy/src/geom", // out-of-source build
        "src/geom"};
    std::unique_ptr<MUGMaterialTable> fMaterialTable;
    std::map<std::string, double> fPhysVolStepLimits;
    std::unique_ptr<G4GenericMessenger> fMessenger;

    G4VPhysicalVolume* fWorld = nullptr;
    G4Region* fSensitiveRegion = new G4Region("SensitiveRegion");
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
