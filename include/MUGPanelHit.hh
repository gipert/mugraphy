#ifndef _MUG_PANEL_HIT_HH_
#define _MUG_PANEL_HIT_HH_

#include <memory>

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class G4GenericMessenger;
class MUGPanelHit : public G4VHit {

  public:

    MUGPanelHit();
    ~MUGPanelHit() = default;

    MUGPanelHit           (MUGPanelHit const&) = delete;
    MUGPanelHit& operator=(MUGPanelHit const&) = delete;
    MUGPanelHit           (MUGPanelHit&&)      = delete;
    MUGPanelHit& operator=(MUGPanelHit&&)      = delete;

    bool operator==(const MUGPanelHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

  private:

    std::unique_ptr<G4GenericMessenger> fMessenger;
    void DefineCommands();
};

typedef G4THitsCollection<MUGPanelHit> MUGPanelHitsCollection;

extern G4ThreadLocal G4Allocator<MUGPanelHit>* MUGPanelHitAllocator;

inline void* MUGPanelHit::operator new(size_t) {
  if(!MUGPanelHitAllocator)
      MUGPanelHitAllocator = new G4Allocator<MUGPanelHit>;
  return (void *) MUGPanelHitAllocator->MallocSingle();
}

inline void MUGPanelHit::operator delete(void *hit) {
  MUGPanelHitAllocator->FreeSingle((MUGPanelHit*) hit);
}

#endif

// vim: tabstop=2 shiftwidth=2 expandtab
