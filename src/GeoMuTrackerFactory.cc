#include "GeoMuTrackerFactory.hh"
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

#include <fstream>

namespace RAT {

G4VPhysicalVolume *GeoMuTrackerFactory::Construct(DBLinkPtr table) {

  G4LogicalVolume *motherLog = FindMother(table->GetS("mother"));
  DBLinkPtr dbinfo = DB::Get()->GetLink("GEO", "MuonTracker");

  G4int enable_mu_tracker = dbinfo->GetI("enable_mu_tracker");

  fibDia = dbinfo->GetD("fiberDiameter");
  fibLen = dbinfo->GetD("fiberLength");
  fScintPanelX = dbinfo->GetD("ScintPanelLenX");
  fScintPanelY = dbinfo->GetD("ScintPanelLenY");
  fScintPanelZ = dbinfo->GetD("ScintPanelLenZ");
  fModuleDis = dbinfo->GetD("ModuleDistance");
  nFibers = dbinfo->GetI("nFibers");
  nSciBars = dbinfo->GetI("nSciBars");
  nOneModule = dbinfo->GetI("nOneModule");
  nModules = dbinfo->GetI("nModules");
  trackerPosition = dbinfo->GetDArray("trackerPosition");

  G4Material *worldMat = G4Material::GetMaterial(table->GetS("worldMaterial"));

  // fModuleDis = 4.0*cm;

  G4double worldZ =
      nOneModule * fScintPanelZ * nModules + (nModules - 1) * fModuleDis;
  muTracker = new G4Box("muTracker", fScintPanelX / 2.0, fScintPanelX / 2.0,
                        worldZ / 2.0);
  muTrackerLogicVol =
      new G4LogicalVolume(muTracker, worldMat, "muTrackerLogicVol");
  muTrackerLogicVol->SetVisAttributes(G4VisAttributes::GetInvisible());

  muTrackerBlock =
      new G4Box("muTrackerBlock", fScintPanelX / 2.0,
                nSciBars * fScintPanelY / 2.0, nOneModule * fScintPanelZ / 2.0);
  muTrackerBlockLV =
      new G4LogicalVolume(muTrackerBlock, worldMat, "muTrackerBlockLV");
  muTrackerBlockLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4double offset = fScintPanelY * nSciBars / 2.0;
  TrackerLayerGeo =
      new G4Box("TrackerLayerGeo", fScintPanelX / 2.0,
                fScintPanelY * nSciBars / 2.0, fScintPanelZ / 2.0);
  TrackerLayerMom =
      new G4LogicalVolume(TrackerLayerGeo, worldMat, "TrackerLayerMom");
  G4VisAttributes *trkLayerVisAtt =
      new G4VisAttributes(G4Colour(0.56, 0.93, 0.56, 0.3));
  TrackerLayerMom->SetVisAttributes(trkLayerVisAtt);
  trkLayerVisAtt->SetVisibility(false);

  WLSfiber = new G4Tubs("WLSfiber", 0.0 * CLHEP::mm, fibDia / 2.0, fibLen / 2.0,
                        0.0 * CLHEP::deg, 360.0 * CLHEP::deg);
  WLSfiberLogicVol = new G4LogicalVolume(
      WLSfiber, G4Material::GetMaterial("wls_fiber"), "WLSfiberLogicVol");
  G4VisAttributes *fiberVisAtt = new G4VisAttributes(G4Colour(0., 0.39, 0.));
  WLSfiberLogicVol->SetVisAttributes(fiberVisAtt);
  fiberVisAtt->SetVisibility(true);

  G4RotationMatrix *rotfib = new G4RotationMatrix();
  rotfib->rotateY(90 * CLHEP::deg);
  scintBox = new G4Box("scintBox", fScintPanelX / 2.0, fScintPanelY / 2.0,
                       fScintPanelZ / 2.0);
  scintPanel = new G4SubtractionSolid(
      "scintPanel", scintBox, WLSfiber, rotfib,
      G4ThreeVector(0.0 * CLHEP::mm, 0.0 * CLHEP::mm, 0.0 * CLHEP::mm));
  scintPanelLogicVol = new G4LogicalVolume(
      scintPanel, G4Material::GetMaterial("polystyrene_scint"), "scintPanel");
  G4VisAttributes *scintPanelVisAtt =
      new G4VisAttributes(G4Colour(0.56, 0.93, 0.56, 0.3));
  scintPanelLogicVol->SetVisAttributes(scintPanelVisAtt);
  scintPanelVisAtt->SetVisibility(true);

  for (int i = 0; i < nSciBars; i++) {
    int fac = 2 * i + 1;
    G4ThreeVector pos = G4ThreeVector(
        0.0 * CLHEP::mm, -offset + fac * fScintPanelY / 2.0, 0.0 * CLHEP::mm);
    new G4PVPlacement(nullptr, pos, scintPanelLogicVol, "trackerLayer",
                      TrackerLayerMom, false, i, true);
    new G4PVPlacement(rotfib, pos, WLSfiberLogicVol, "fiberLayer",
                      TrackerLayerMom, false, i, true);
  }

  // each block(module) has 4 layers
  offset = fScintPanelZ * nOneModule / 2.0;

  for (int i = 0; i < nOneModule; i++) {

    G4ThreeVector pos =
        G4ThreeVector(0.0 * CLHEP::mm, 0.0 * CLHEP::mm,
                      -offset + fScintPanelZ / 2.0 + i * fScintPanelZ);
    G4RotationMatrix *rot = new G4RotationMatrix();
    rot->rotateZ(90 * i * CLHEP::deg);

    new G4PVPlacement(rot, pos, TrackerLayerMom, "trackerBlockPV",
                      muTrackerBlockLV, false, i, true);
  }

  G4double oneModuleLen = fScintPanelZ * nOneModule;
  for (int i = 0; i < nModules; i++) {
    G4ThreeVector pos = G4ThreeVector(
        0.0 * CLHEP::mm, 0.0 * CLHEP::mm,
        -worldZ / 2.0 + (2 * i + 1) * oneModuleLen / 2.0 + i * fModuleDis);
    new G4PVPlacement(nullptr, pos, muTrackerBlockLV, "trackerPV",
                      muTrackerLogicVol, false, i, true);
  }

  G4ThreeVector trackerPos(trackerPosition[0], trackerPosition[1],
                           trackerPosition[2]);

  G4VPhysicalVolume *MuonTrackerPV =
      new G4PVPlacement(nullptr, trackerPos, muTrackerLogicVol, "MuonTrackerPV",
                        motherLog, false, 0, true);

  return MuonTrackerPV;
  // return NULL;
}

/*
void GeoMuTrackerFactory::SetSensitive(){

}
*/

} // namespace RAT
