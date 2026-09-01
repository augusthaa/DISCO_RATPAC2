#include "GeoPMTHousingFactory.hh"
#include <RAT/DB.hh>
#include <RAT/Log.hh>
#include <RAT/Materials.hh>

#include <CLHEP/Units/SystemOfUnits.h>
#include <G4Box.hh>
#include <G4Color.hh>
#include <G4Ellipsoid.hh>
#include <G4GDMLParser.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4Material.hh>
#include <G4OpticalSurface.hh>
#include <G4PVPlacement.hh>
#include <G4Polyhedra.hh>
#include <G4SubtractionSolid.hh>
#include <G4Tubs.hh>
#include <G4UnionSolid.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VSolid.hh>
#include <vector>

namespace RAT {

G4VPhysicalVolume *GeoPMTHousingFactory::Construct(DBLinkPtr table) {

  G4LogicalVolume *motherLog = FindMother(table->GetS("mother"));
  DBLinkPtr dbinfo = DB::Get()->GetLink("GEO", "PMTHousing");

  PMThousingPosition = dbinfo->GetDArray("PMThousingPosition");
  rmin_adit = dbinfo->GetD("rmin_adit");
  rmax_adit = dbinfo->GetD("rmax_adit");
  rmin_hamamatsu = dbinfo->GetD("rmin_hamamatsu");
  rmax_hamamatsu = dbinfo->GetD("rmax_hamamatsu");
  G4Material *worldMat = G4Material::GetMaterial(table->GetS("worldMaterial"));

  const G4int numZPlanes = 8;
  G4double z_plane[numZPlanes] = {0.0,     29.0001, 30.0,     40.0,
                                  40.0001, 399.0,   399.0001, 400.0};
  G4double r_inner[numZPlanes] = {170.0, 170.0, 150.0, 150.0,
                                  170.0, 170.0, 0.0,   0.0};
  G4double r_outer[numZPlanes] = {180.0, 180.0, 180.0, 180.0,
                                  180.0, 180.0, 180.0, 180.0};

  // G4double z_plane[numZPlanes] = {400.0, 399.0001,
  // 399.0, 40.0001, 40.0, 30.0, 29.0001, 0.0}; G4double r_inner[numZPlanes] =
  // {0.0, 0.0, 170.0, 170.0, 150.0, 150.0, 170.0, 170.0}; G4double
  // r_outer[numZPlanes] = {180.0, 180.0, 180.0, 180.0, 180.0, 180.0, 180.0,
  // 180.0};

  G4Tubs *PMTHousingGeo =
      new G4Tubs("PMTHousingGeo", 0.0 * CLHEP::mm, 180.0 * CLHEP::mm,
                 200.0 * CLHEP::mm, 0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  discoPMTHousing =
      new G4LogicalVolume(PMTHousingGeo, worldMat, "discoPMTHousing");
  G4VisAttributes *h_col = new G4VisAttributes(G4Colour(0., 0.0, 1.0));
  discoPMTHousing->SetVisAttributes(h_col);
  h_col->SetVisibility(false);

  G4Polycone *barrelGeo =
      new G4Polycone("barrelGeo", 0.0 * CLHEP::deg, 360.0 * CLHEP::deg, 8,
                     z_plane, r_inner, r_outer);
  pmtHousingBarrelLV =
      new G4LogicalVolume(barrelGeo, G4Material::GetMaterial("stainless_steel"),
                          "pmtHousingBarrelLV");
  G4VisAttributes *b_col = new G4VisAttributes(G4Colour(0., 0.39, 0.));
  pmtHousingBarrelLV->SetVisAttributes(b_col);
  b_col->SetVisibility(true);

  new G4PVPlacement(
      nullptr,
      G4ThreeVector(0.0 * CLHEP::mm, 0.0 * CLHEP::mm, -200.0 * CLHEP::mm),
      pmtHousingBarrelLV, "pmtHousingBarrelPV", discoPMTHousing, false, 0,
      true);

  G4VSolid *PMTtopPlateGeo =
      new G4Tubs("PMTtopPlateGeo", 0.0 * CLHEP::mm, 150.0 * CLHEP::mm,
                 10.0 * CLHEP::mm, 0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  G4Tubs *adit_hole_geo =
      new G4Tubs("adit_hole_geo", 0.0, rmax_adit, 11.0 * CLHEP::mm,
                 0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  G4Tubs *hamamatsu_hole_geo =
      new G4Tubs("hamamatsu_hole_geo", 0.0, rmax_hamamatsu, 11.0 * CLHEP::mm,
                 0.0 * CLHEP::deg, 360.0 * CLHEP::deg);

  G4Tubs *aditPMTHolderGeo =
      new G4Tubs("aditPMTHolderGeo", rmin_adit, rmax_adit, 25.0 * CLHEP::mm,
                 0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  aditPMTHolderLV = new G4LogicalVolume(
      aditPMTHolderGeo, G4Material::GetMaterial("nylon"), "aditPMTHolderLV");

  G4Tubs *hamamatsuPMTHolderGeo =
      new G4Tubs("hamamatsuPMTHolderGeo", rmin_hamamatsu, rmax_hamamatsu,
                 25.0 * CLHEP::mm, 0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  hamamatsuPMTHolderLV =
      new G4LogicalVolume(hamamatsuPMTHolderGeo,
                          G4Material::GetMaterial("nylon"), "aditPMTHolderLV");

  G4VSolid *PMTHolderPlate = PMTtopPlateGeo;

  PMTHolderPlate = new G4SubtractionSolid("PMTHolderPlate", PMTHolderPlate,
                                          hamamatsu_hole_geo, nullptr,
                                          G4ThreeVector(0.0, 0.0, 0.0));

  for (int i = 0; i < 5; i++) {
    G4double phi = i * CLHEP::twopi / 5;
    G4ThreeVector hPosition(40.0 * std::cos(phi), 40.0 * std::sin(phi),
                            0.0 * CLHEP::mm);
    PMTHolderPlate =
        new G4SubtractionSolid("PMTHolderPlate", PMTHolderPlate,
                               hamamatsu_hole_geo, nullptr, hPosition);
    // new G4PVPlacement(nullptr, hPosition, hamamatsuPMTHolderLV,
    // "PMTHoldersPV", discoPMTHousing, false, i+1, true);
  }

  for (int i = 0; i < 10; i++) {
    G4double phi = i * CLHEP::twopi / 10;
    G4ThreeVector hPosition(115.0 * std::cos(phi), 115.0 * std::sin(phi),
                            0.0 * CLHEP::mm);
    PMTHolderPlate = new G4SubtractionSolid("PMTHolderPlate", PMTHolderPlate,
                                            adit_hole_geo, nullptr, hPosition);
    // new G4PVPlacement(nullptr, hPosition, aditPMTHolderLV, "PMTHoldersPV",
    // discoPMTHousing, false, i+6, true);
  }

  PMTtopPlateLV = new G4LogicalVolume(PMTHolderPlate,
                                      G4Material::GetMaterial("acrylic_black"),
                                      "PMTtopPlateLV");

  // use z = 25 mm when using PMTtopPlateLV as the mother volume, else z = -145
  // + 5mm thickness of top plate
  for (int i = 0; i < 16; i++) {

    if (i == 0) {
      G4ThreeVector hPosition(0.0 * CLHEP::mm, 0.0 * CLHEP::mm,
                              -155.0 * CLHEP::mm); // was z = 25 mm
      // new G4PVPlacement(nullptr, hPosition, aditPMTHolderLV, "PMTHoldersPV",
      // PMTtopPlateLV, false, i, true);
      new G4PVPlacement(nullptr, hPosition, hamamatsuPMTHolderLV,
                        "PMTHoldersPV", discoPMTHousing, false, i, true);
    }

    if (i > 0 && i < 6) {
      G4double phi = (i - 1) * CLHEP::twopi / 5.0;
      G4double PMTrad = 40.0 * CLHEP::mm;
      G4ThreeVector hPosition(PMTrad * std::cos(phi), PMTrad * std::sin(phi),
                              -155.0 * CLHEP::mm);
      // new G4PVPlacement(nullptr, hPosition, aditPMTHolderLV, "PMTHoldersPV",
      // PMTtopPlateLV, false, i, true);
      new G4PVPlacement(nullptr, hPosition, hamamatsuPMTHolderLV,
                        "PMTHoldersPV", discoPMTHousing, false, i, true);
    }

    if (i > 5 && i < 16) {
      G4double phi = (i - 6) * CLHEP::twopi / 10.0;
      G4double PMTrad = 115.0 * CLHEP::mm;
      G4ThreeVector hPosition(PMTrad * std::cos(phi), PMTrad * std::sin(phi),
                              -155.0 * CLHEP::mm);
      // new G4PVPlacement(nullptr, hPosition, aditPMTHolderLV, "PMTHoldersPV",
      // PMTtopPlateLV, false, i, true);
      new G4PVPlacement(nullptr, hPosition, aditPMTHolderLV, "PMTHoldersPV",
                        discoPMTHousing, false, i, true);
    }
  }

  G4RotationMatrix *rot = new G4RotationMatrix();
  rot->rotateX(180.0 * CLHEP::deg);

  G4VisAttributes *plate_col = new G4VisAttributes(G4Colour(1., 0., 0., 0.3));
  PMTtopPlateLV->SetVisAttributes(plate_col);
  new G4PVPlacement(
      nullptr,
      G4ThreeVector(0.0 * CLHEP::mm, 0.0 * CLHEP::mm, -170.0 * CLHEP::mm),
      PMTtopPlateLV, "PMTtopPlatePV", discoPMTHousing, false, 0, true);

  G4VPhysicalVolume *PMTHousingPV = new G4PVPlacement(
      rot,
      G4ThreeVector(PMThousingPosition[0], PMThousingPosition[1],
                    PMThousingPosition[2]),
      discoPMTHousing, "PMTHousingPV", motherLog, false, 0, true);

  return PMTHousingPV;
  // return NULL;
}

} // namespace RAT
