#ifndef __RAT_GeoMuTrackerSDHit__
#define __RAT_GeoMuTrackerSDHit__

#include <G4Allocator.hh>
#include <G4LogicalVolume.hh>
#include <G4RotationMatrix.hh>
#include <G4THitsCollection.hh>
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <G4VHit.hh>

namespace RAT {

class GeoMuTrackerSDHit : public G4VHit {
public:
  GeoMuTrackerSDHit();
  GeoMuTrackerSDHit(G4int i, G4double t, G4ThreeVector p, G4double e,
                    G4int _pdg);
  virtual ~GeoMuTrackerSDHit();
  GeoMuTrackerSDHit(const GeoMuTrackerSDHit &right);
  const GeoMuTrackerSDHit &operator=(const GeoMuTrackerSDHit &right);
  int operator==(const GeoMuTrackerSDHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  void Draw();
  void Print();

private:
  G4int id;
  G4double time;
  G4ThreeVector pos;
  G4int pdg;
  const G4LogicalVolume *pLogV;
  G4String proc;
  G4double Edep;
  G4String LVname;

public:
  inline G4int GetID() const { return id; }
  inline void SetID(G4int sID) { id = sID; }
  inline G4double GetTime() const { return time; }
  inline void SetTime(G4double val) { time = val; }
  inline void SetPos(G4ThreeVector xyz) { pos = xyz; }
  inline G4ThreeVector GetPos() const { return pos; }
  inline void SetLogV(G4LogicalVolume *val) { pLogV = val; }
  inline const G4LogicalVolume *GetLogV() const { return pLogV; }
  inline void SetLogVName(G4String val) { LVname = val; }
  inline const G4String GetLogVName() const { return LVname; }
  inline void SetPDG(G4int _pdg) { pdg = _pdg; }
  inline G4int GetPDG() const { return pdg; }
  inline void SetHitProcessName(G4String processName) { proc = processName; }
  inline G4String GetHitProcessName() { return proc; }
  inline void SetEdep(G4double energy) { Edep = energy; }
  inline G4double GetEdep() { return Edep; }
};

typedef G4THitsCollection<GeoMuTrackerSDHit> GeoMuTrackerSDHitsCollection;

extern G4Allocator<GeoMuTrackerSDHit> GeoMuTrackerSDHitAllocator;

inline void *GeoMuTrackerSDHit::operator new(size_t) {
  void *aHit;
  aHit = (void *)GeoMuTrackerSDHitAllocator.MallocSingle();
  return aHit;
}

inline void GeoMuTrackerSDHit::operator delete(void *aHit) {
  GeoMuTrackerSDHitAllocator.FreeSingle((GeoMuTrackerSDHit *)aHit);
}

} // namespace RAT

#endif
