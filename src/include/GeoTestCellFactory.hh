#ifndef __RAT_GeoTestCellFactory__
#define __RAT_GeoTestCellFactory__

#include <RAT/GeoFactory.hh>

#include <G4Box.hh>
#include <G4EllipticalTube.hh>
#include <G4OpticalSurface.hh>
#include <G4Polyhedra.hh>
#include <G4Sphere.hh>
#include <G4SubtractionSolid.hh>
#include <G4Tubs.hh>
#include <G4UnionSolid.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VisAttributes.hh>

namespace RAT {
class GeoTestCellFactory : public GeoFactory {
public:
  GeoTestCellFactory() : GeoFactory("discoTestCell"){};
  virtual G4VPhysicalVolume *Construct(DBLinkPtr table);

protected:
  G4double fTestCellHeight, fCellDia, fCellThickness, fAcrylicThickness,
      fTopCoverThickness;
  G4double fDomeThickness, fDetMediumHeight;
  G4int enable_dome;
  std::vector<G4double> TestCellPosition;
  G4String fDetMedium;

  G4Tubs *TestCellGeo, *TCWorldGeo;
  G4Sphere *domeGeo, *insideDomeGeo;
  G4LogicalVolume *TestCellLV, *acrylicLV, *domeLV, *airgapLV, *detMediumLV,
      *motherLogicWorld, *insideDomeLV, *blackDiscLV, *TCWorldLV;
  G4EllipticalTube *acrylicGeo, *airgapGeo, *detMediumGeo, *blackDiscGeo;
  G4VPhysicalVolume *TestCellPV, *acrylicPV, *airgapPV, *domePV, *detMediumPV,
      *insideDomePV, *blackDiscPV, *acrylicBotPV, *acrylicTopPV;
};

} // namespace RAT

#endif
