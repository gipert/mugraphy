#ifndef _MUG_MATERIAL_TABLE_HH_
#define _MUG_MATERIAL_TABLE_HH_

#include <map>
#include <memory>

#include "G4GenericMessenger.hh"
#include "globals.hh"

class G4Material;
class MUGMaterialTable {

  public:

    MUGMaterialTable();
    ~MUGMaterialTable() = default;

    MUGMaterialTable(MUGMaterialTable const&) = delete;
    MUGMaterialTable& operator=(MUGMaterialTable const&) = delete;
    MUGMaterialTable(MUGMaterialTable&&) = delete;
    MUGMaterialTable& operator=(MUGMaterialTable&&) = delete;

    static G4Material* GetMaterial(std::string);

  private:

    void InitializeMaterials();

    std::unique_ptr<G4GenericMessenger> fMessenger = nullptr;
    std::unique_ptr<G4GenericMessenger> fLArMessenger = nullptr;
    void DefineCommands();

    static std::map<std::string, std::string> fMaterialAliases;
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
