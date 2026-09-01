#ifndef __RAT_GeoMuTrackerFactory__
#define __RAT_GeoMuTrackerFactory__

#include <RAT/GeoFactory.hh>

#include <G4Box.hh>
#include <G4OpticalSurface.hh>
#include <G4Polyhedra.hh>
#include <G4SubtractionSolid.hh>
#include <G4Tubs.hh>
#include <G4UnionSolid.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VisAttributes.hh>

namespace RAT {
class GeoMuTrackerFactory : public GeoFactory {
public:
  GeoMuTrackerFactory() : GeoFactory("discoMuonTracker"){};
  virtual G4VPhysicalVolume *Construct(DBLinkPtr table);
  std::vector<G4double> trackerPosition;

private:
  virtual void SetSensitive(G4LogicalVolume *logi, DBLinkPtr table);

protected:
  G4double fibDia, fibLen;
  G4double fScintPanelX, fScintPanelY, fScintPanelZ;
  G4int nFibers, nSciBars, nOneModule, nModules;
  G4double fModuleDis;

  G4LogicalVolume *muTrackerLogicVol, *WLSfiberLogicVol, *scintPanelLogicVol,
      *TrackerLayerMom, *muTrackerBlockLV;
  G4Tubs *WLSfiber;
  G4SubtractionSolid *scintPanel;
  G4Box *scintBox, *TrackerLayerGeo, *muTrackerBlock, *muTracker;
};

} // namespace RAT

#endif
