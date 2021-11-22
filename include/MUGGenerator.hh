#ifndef _MUG_GENERATOR_PRIMARY_HH_
#define _MUG_GENERATOR_PRIMARY_HH_

#include <memory>

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4GenericMessenger;
class MUGGenerator : public G4VUserPrimaryGeneratorAction {

  public:

    MUGGenerator();
    ~MUGGenerator() = default;

    MUGGenerator           (MUGGenerator const&) = delete;
    MUGGenerator& operator=(MUGGenerator const&) = delete;
    MUGGenerator           (MUGGenerator&&)      = delete;
    MUGGenerator& operator=(MUGGenerator&&)      = delete;

    void GeneratePrimaries(G4Event *event) override;

  private:

    std::unique_ptr<G4GenericMessenger> fMessenger;
    void DefineCommands();
};

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
