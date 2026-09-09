#ifndef __RAT_GeoMuTrackerSD__
#define __RAT_GeoMuTrackerSD__

#include "GeoMuTrackerSDHit.hh"
#include <G4VSensitiveDetector.hh>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

namespace RAT {

class GeoMuTrackerSD : public G4VSensitiveDetector {
public:
  GeoMuTrackerSD(G4String name);
  virtual ~GeoMuTrackerSD();

  virtual void Initialize(G4HCofThisEvent *HCE);
  virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);
  virtual void EndOfEvent(G4HCofThisEvent *HCE);

  // Data members which are publicly accessible and can be
  // written out to the RAT event tree

  G4ThreeVector _hit_pos;
  /** hit position */
  G4double _hit_E;
  /** hit energy deposition */
  G4double _hit_time;
  /** global time of the hit */
  G4int _hit_pdg;
  /** pdg of particle that left the hit */
  G4String _hit_volume;
  /** name of volume of hit */
  G4int _hit_panel_id;
  /** ID of the hit panel */
  G4String _hit_process_name;

private:
  int fLastEventID;
  int fLastTrackID;

  GeoMuTrackerSDHitsCollection *_hitsCollection;
  G4int HCID;
  G4HCofThisEvent *_HCE;
};

} // namespace RAT

#endif
