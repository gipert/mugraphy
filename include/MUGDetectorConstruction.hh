#ifndef _MUG_MANAGEMENT_DETECTOR_CONSTRUCTION_HH_
#define _MUG_MANAGEMENT_DETECTOR_CONSTRUCTION_HH_

#include <map>
#include <memory>
#include <vector>

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include "MUGMaterialTable.hh"
#include "MUGNavigationTools.hh"

class G4VPhysicalVolume;
class G4GenericMessenger;
class G4VPhysicalVolume;
class MUGDetectorConstruction : public G4VUserDetectorConstruction {

  public:

    MUGDetectorConstruction();
    ~MUGDetectorConstruction() = default;

    MUGDetectorConstruction           (MUGDetectorConstruction const&) = delete;
    MUGDetectorConstruction& operator=(MUGDetectorConstruction const&) = delete;
    MUGDetectorConstruction           (MUGDetectorConstruction&&)      = delete;
    MUGDetectorConstruction& operator=(MUGDetectorConstruction&&)      = delete;

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

    std::vector<std::string> fGDMLFiles;
    std::unique_ptr<MUGMaterialTable> fMaterialTable;
    std::map<std::string, double> fPhysVolStepLimits;
    std::unique_ptr<G4GenericMessenger> fMessenger;
    void DefineCommands();

    G4VPhysicalVolume* fWorld;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
