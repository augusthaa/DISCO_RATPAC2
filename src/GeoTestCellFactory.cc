#include "GeoTestCellFactory.hh"
#include <RAT/DB.hh>
#include <RAT/Log.hh>
#include <RAT/Materials.hh>

#include <CLHEP/Units/SystemOfUnits.h>
#include <G4Color.hh>
#include <G4GDMLParser.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4OpticalSurface.hh>
#include <G4PVPlacement.hh>
#include <G4VPhysicalVolume.hh>
#include <vector>

#include <fstream>

namespace RAT {

G4VPhysicalVolume *GeoTestCellFactory::Construct(DBLinkPtr table) {

  G4LogicalVolume *motherLog = FindMother(table->GetS("mother"));
  DBLinkPtr dbinfo = DB::Get()->GetLink("GEO", "TestCell");

  fTestCellHeight = dbinfo->GetD("TestCellHeight");
  fCellDia = dbinfo->GetD("CellDiameter");
  fCellThickness = dbinfo->GetD("CellThickness");
  fAcrylicThickness = dbinfo->GetD("AcrylicThickness");
  fTopCoverThickness = dbinfo->GetD("TopCoverThickness");
  TestCellPosition = dbinfo->GetDArray("TestCellPosition");
  fDetMediumHeight = dbinfo->GetD("DetMediumHeight");
  fDetMedium = dbinfo->GetS("detection_medium");

  enable_dome = dbinfo->GetI("enable_dome");

  TCWorldGeo = new G4Tubs("TestCellGeo", 0.0, fCellDia / 2.0,
                          fTestCellHeight / 2.0 + fTopCoverThickness / 2.0,
                          0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  TCWorldLV = new G4LogicalVolume(TCWorldGeo, G4Material::GetMaterial("G4_AIR"),
                                  "TCWorldLV");
  TCWorldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  // stainless steel test cell
  TestCellGeo =
      new G4Tubs("TestCellGeo", fCellDia / 2.0 - fCellThickness, fCellDia / 2.0,
                 fTestCellHeight / 2.0, 0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  TestCellLV = new G4LogicalVolume(
      TestCellGeo, G4Material::GetMaterial("stainless_steel"), "TestCellLV");
  G4VisAttributes *cell_col = new G4VisAttributes(G4Colour(1., 0., 0., 0.3));
  TestCellLV->SetVisAttributes(cell_col);
  G4ThreeVector barrelPosition = G4ThreeVector(0.0 * CLHEP::mm, 0.0 * CLHEP::mm,
                                               -fTopCoverThickness / 2.0);
  TestCellPV = new G4PVPlacement(nullptr, barrelPosition, TestCellLV,
                                 "TestCellPV", TCWorldLV, false, 0, true);

  // black disc on the top of the vessel
  blackDiscGeo = new G4EllipticalTube("blackDiscGeo", fCellDia / 2.0,
                                      fCellDia / 2.0, fTopCoverThickness / 2.0);
  blackDiscLV = new G4LogicalVolume(
      blackDiscGeo, G4Material::GetMaterial("acrylic_black"), "blackDiscLV");
  G4VisAttributes *blackDisc_col =
      new G4VisAttributes(G4Colour(1., 0.5, 1., 0.3));
  blackDiscLV->SetVisAttributes(blackDisc_col);
  G4ThreeVector blackCoverPosition =
      G4ThreeVector(0.0 * CLHEP::mm, 0.0 * CLHEP::mm, fTestCellHeight / 2.0);
  blackDiscPV = new G4PVPlacement(nullptr, blackCoverPosition, blackDiscLV,
                                  "blackDiscPV", TCWorldLV, false, 0, true);

  // acrylic discs at the top and bottom of the vessel
  acrylicGeo = new G4EllipticalTube(
      "acrylicGeo", fCellDia / 2.0 - fCellThickness,
      fCellDia / 2.0 - fCellThickness, fAcrylicThickness / 2.0);
  acrylicLV = new G4LogicalVolume(
      acrylicGeo, G4Material::GetMaterial("acrylic_uvt"), "acrylicLV");
  G4VisAttributes *acr_col = new G4VisAttributes(G4Colour(1., 1., 1., 0.3));
  acrylicLV->SetVisAttributes(acr_col);
  G4ThreeVector pos_acr_top =
      G4ThreeVector(0.0 * CLHEP::mm, 0.0 * CLHEP::mm,
                    fTestCellHeight / 2.0 - fTopCoverThickness / 2.0 -
                        fAcrylicThickness / 2.0);
  G4ThreeVector pos_acr_bot =
      G4ThreeVector(0.0 * CLHEP::mm, 0.0 * CLHEP::mm,
                    -fTestCellHeight / 2.0 - fTopCoverThickness / 2.0 +
                        fAcrylicThickness / 2.0);
  acrylicTopPV = new G4PVPlacement(nullptr, pos_acr_top, acrylicLV, "acrylicPV",
                                   TCWorldLV, false, 0, true);
  acrylicBotPV = new G4PVPlacement(nullptr, pos_acr_bot, acrylicLV, "acrylicPV",
                                   TCWorldLV, false, 1, true);

  // air inside test cell
  G4double fAirRad = fCellDia / 2.0 - fCellThickness;
  G4double airHeightInCell =
      fTestCellHeight - 2.0 * fAcrylicThickness - fDetMediumHeight;
  if (airHeightInCell > 0.0) {
    airgapGeo = new G4EllipticalTube("airgapGeo", fAirRad, fAirRad,
                                     airHeightInCell / 2.0);
    airgapLV = new G4LogicalVolume(airgapGeo, G4Material::GetMaterial("air"),
                                   "airgapLV");
    G4VisAttributes *airgap_col =
        new G4VisAttributes(G4Colour(0., 0., 1., 0.3));
    airgapLV->SetVisAttributes(airgap_col);
    G4ThreeVector pos_air = G4ThreeVector(
        0.0 * CLHEP::mm, 0.0 * CLHEP::mm,
        -fTestCellHeight / 2.0 - fTopCoverThickness / 2.0 + fAcrylicThickness +
            fDetMediumHeight + airHeightInCell / 2.0);
    airgapPV = new G4PVPlacement(nullptr, pos_air, airgapLV, "airgapPV",
                                 TCWorldLV, false, 0, true);
  }

  // detection medium
  detMediumGeo = new G4EllipticalTube("detMediumGeo", fAirRad, fAirRad,
                                      fDetMediumHeight / 2.0);
  detMediumLV = new G4LogicalVolume(
      detMediumGeo, G4Material::GetMaterial(fDetMedium), "detMediumLV");
  G4VisAttributes *detmed_col = new G4VisAttributes(G4Colour(0., 0., 1., 0.5));
  detMediumLV->SetVisAttributes(detmed_col);
  G4ThreeVector pos_det_med =
      G4ThreeVector(0.0 * CLHEP::mm, 0.0 * CLHEP::mm,
                    -fTestCellHeight / 2.0 - fTopCoverThickness / 2.0 +
                        fAcrylicThickness + fDetMediumHeight / 2.0);
  detMediumPV = new G4PVPlacement(nullptr, pos_det_med, detMediumLV,
                                  "detMediumPV", TCWorldLV, false, 0, true);

  G4ThreeVector TestCellPos(TestCellPosition[0], TestCellPosition[1],
                            TestCellPosition[2]);
  G4VPhysicalVolume *TestCellPV = new G4PVPlacement(
      nullptr, TestCellPos, TCWorldLV, "TestCellPV", motherLog, false, 0, true);

  // surfaces
  // nylon sheets or relfectlectors
  // they are treated as a film that absorbs/reflects
  G4OpticalSurface *cellBarrelCoating =
      new G4OpticalSurface("cellBarrelCoating");
  cellBarrelCoating->SetType(dielectric_metal);

  // if diffuse reflector
  cellBarrelCoating->SetFinish(ground);
  cellBarrelCoating->SetModel(glisur);

  // if specular reflections
  // cellBarrelCoating->SetFinish(polished);
  // cellBarrelCoating->SetModel(glisur);

  G4LogicalSkinSurface *coating =
      new G4LogicalSkinSurface("coating", TestCellLV, cellBarrelCoating);

  // water - steel interfacte
  G4OpticalSurface *opWaterSteel = new G4OpticalSurface("opWaterSteel");
  opWaterSteel->SetType(dielectric_dielectric);
  opWaterSteel->SetModel(unified);
  opWaterSteel->SetFinish(polished);
  G4LogicalBorderSurface *waterSteelSurface = new G4LogicalBorderSurface(
      "waterSteelSurface", detMediumPV, TestCellPV, opWaterSteel);

  // water - acrylic interface
  G4OpticalSurface *opWaterAcrylic = new G4OpticalSurface("opWaterAcrylic");
  opWaterAcrylic->SetType(dielectric_dielectric);
  opWaterAcrylic->SetModel(unified);
  opWaterAcrylic->SetFinish(polished);
  G4LogicalBorderSurface *waterAcrylicSurface = new G4LogicalBorderSurface(
      "waterAcrylicSurface", detMediumPV, acrylicBotPV, opWaterAcrylic);

  if (airHeightInCell > 0.0) {

    // acrylic - air interface
    G4OpticalSurface *opAcrylicAir = new G4OpticalSurface("opAcrylicAir");
    opAcrylicAir->SetType(dielectric_dielectric);
    opAcrylicAir->SetFinish(polished);
    opAcrylicAir->SetModel(unified);
    G4LogicalBorderSurface *acrylicAirSurface = new G4LogicalBorderSurface(
        "acrylicAirSurface", acrylicTopPV, airgapPV, opAcrylicAir);

    // steel - air interface
    G4OpticalSurface *opAirSteel = new G4OpticalSurface("opAirSteel");
    opAirSteel->SetType(dielectric_dielectric);
    opAirSteel->SetModel(unified);
    opAirSteel->SetFinish(polished);
    G4LogicalBorderSurface *airSteelSurface = new G4LogicalBorderSurface(
        "airSteelSurface", airgapPV, TestCellPV, opAirSteel);

    // water - air interface
    G4OpticalSurface *opWaterAir = new G4OpticalSurface("opWaterAir");
    opWaterAir->SetType(dielectric_dielectric);
    opWaterAir->SetModel(unified);
    opWaterAir->SetFinish(polished);
    G4LogicalBorderSurface *waterAirSurface = new G4LogicalBorderSurface(
        "waterAirSurface", detMediumPV, airgapPV, opWaterAir);

  } else {

    // water - acrylic interface
    G4OpticalSurface *opWaterAcrylic = new G4OpticalSurface("opWaterAcrylic");
    opWaterAcrylic->SetType(dielectric_dielectric);
    opWaterAcrylic->SetModel(unified);
    opWaterAcrylic->SetFinish(polished);
    G4LogicalBorderSurface *waterAcrylicSurface = new G4LogicalBorderSurface(
        "waterAcrylicSurface", acrylicTopPV, detMediumPV, opWaterAcrylic);
  }

  return TestCellPV;
  // return NULL;
}

} // namespace RAT
