#ifndef _MUG_MATERIAL_TABLE_HH_
#define _MUG_MATERIAL_TABLE_HH_

#include <memory>
#include <map>

#include "globals.hh"
#include "G4GenericMessenger.hh"

class G4Material;
class MUGMaterialTable {

  public:

    MUGMaterialTable();
    ~MUGMaterialTable() = default;

    MUGMaterialTable           (MUGMaterialTable const&) = delete;
    MUGMaterialTable& operator=(MUGMaterialTable const&) = delete;
    MUGMaterialTable           (MUGMaterialTable&&)      = delete;
    MUGMaterialTable& operator=(MUGMaterialTable&&)      = delete;

    static G4Material* GetMaterial(std::string);

  private:

    void InitializeMaterials();

    std::unique_ptr<G4GenericMessenger> fMessenger;
    std::unique_ptr<G4GenericMessenger> fLArMessenger;
    void DefineCommands();

    static std::map<std::string, std::string> fMaterialAliases;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
