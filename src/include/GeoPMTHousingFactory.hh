#ifndef __RAT_PMTHousingFactory__
#define __RAT_PMTHousingFactory__

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
class GeoPMTHousingFactory : public GeoFactory {
public:
  GeoPMTHousingFactory() : GeoFactory("discoPMTHousing"){};
  virtual G4VPhysicalVolume *Construct(DBLinkPtr table);

protected:
  G4double rmin_adit, rmax_adit, rmin_hamamatsu, rmax_hamamatsu;
  std::vector<G4double> PMThousingPosition;

  G4LogicalVolume *pmtHousingBarrelLV, *PMTtopPlateLV, *discoPMTHousing,
      *hamamatsuPMTHolderLV, *aditPMTHolderLV;
};

} // namespace RAT

#endif
