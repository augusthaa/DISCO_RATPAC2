#include <CLHEP/Units/SystemOfUnits.h>

#include "GeoMuTrackerSDHit.hh"
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VVisManager.hh>
#include <G4VisAttributes.hh>
#include <G4ios.hh>
#include <RAT/Log.hh>

namespace RAT {

G4Allocator<GeoMuTrackerSDHit> GeoMuTrackerSDHitAllocator;

GeoMuTrackerSDHit::GeoMuTrackerSDHit() {
  id = -1;
  time = 0.;
  pos = G4ThreeVector();
  pdg = 0;
  Edep = 0.;
  pLogV = 0;
}

GeoMuTrackerSDHit::GeoMuTrackerSDHit(G4int i, G4double t, G4ThreeVector p,
                                     G4double e, G4int _pdg) {
  id = i;
  time = t;
  pos = p;
  pdg = _pdg;
  Edep = e;
  pLogV = 0;
}

GeoMuTrackerSDHit::~GeoMuTrackerSDHit() { ; }

GeoMuTrackerSDHit::GeoMuTrackerSDHit(const GeoMuTrackerSDHit &right)
    : G4VHit() {
  id = right.id;
  time = right.time;
  pos = right.pos;
  Edep = right.Edep;
  pLogV = right.pLogV;
  pdg = right.pdg;
  proc = right.proc;
  LVname = right.LVname;
}

const GeoMuTrackerSDHit &
GeoMuTrackerSDHit::operator=(const GeoMuTrackerSDHit &right) {

  id = right.id;
  time = right.time;
  pos = right.pos;
  Edep = right.Edep;
  pLogV = right.pLogV;
  pdg = right.pdg;
  proc = right.proc;
  LVname = right.LVname;
  return *this;
}

int GeoMuTrackerSDHit::operator==(const GeoMuTrackerSDHit & /*right*/) const {
  return 0;
}

void GeoMuTrackerSDHit::Draw() {}

void GeoMuTrackerSDHit::Print() {
  debug << " Muon tracker hit [" << id << "] " << time / CLHEP::ns << " (nsec)"
        << newline;
}

} // namespace RAT
