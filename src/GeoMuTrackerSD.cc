#include "GeoMuTrackerSD.hh"
#include "GeoMuTrackerSDHit.hh"
#include <G4HCofThisEvent.hh>
#include <G4OpticalPhoton.hh>
#include <G4Run.hh>
#include <G4RunManager.hh>
#include <G4SDManager.hh>
#include <G4Step.hh>
#include <G4TouchableHistory.hh>
#include <G4Track.hh>
#include <G4UIcommand.hh>
#include <G4UImanager.hh>
#include <G4UnitsTable.hh>
#include <G4ios.hh>
#include <RAT/Log.hh>

namespace RAT {

GeoMuTrackerSD::GeoMuTrackerSD(G4String name, G4int nSciBars, G4int nOneModule)
    : G4VSensitiveDetector(name), fNSciBars(nSciBars), fNOneModule(nOneModule) {
  G4String HCname;
  collectionName.insert(HCname = "MuTrackerHits");
  HCID = -1;
}

GeoMuTrackerSD::~GeoMuTrackerSD() { ; }

void GeoMuTrackerSD::Initialize(G4HCofThisEvent *HCE) {
  debug << "GeoMuTrackerSD::Initialize start." << newline;
  _hitsCollection = new GeoMuTrackerSDHitsCollection(SensitiveDetectorName,
                                                     collectionName[0]);

  debug << "GeoMuTrackerSD::Initialize hit collection address is "
        << _hitsCollection << newline;
  if (HCID < 0) {
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(_hitsCollection);
  }
  debug << "GeoMuTrackerSD::Initialize hit collection ID = " << HCID << newline;
  HCE->AddHitsCollection(HCID, _hitsCollection);

  // store pointer to hit collection
  _HCE = HCE;

  debug << "GeoMuTrackerSD::Initialize end." << newline;
}

G4bool GeoMuTrackerSD::ProcessHits(G4Step *aStep,
                                   G4TouchableHistory * /*ROhist*/) {

  G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
  G4Track *aTrack = aStep->GetTrack();
  G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
  const G4VTouchable *touchable = preStepPoint->GetTouchable();

  // only proceed if the energy deposited is non zero
  auto edep = aStep->GetTotalEnergyDeposit() / CLHEP::MeV;
  if (edep == 0.0)
    return false;

  G4ParticleDefinition *particleType = aTrack->GetDefinition();

  if (particleType == G4OpticalPhoton::OpticalPhotonDefinition()) {
    return true;
  }

  int trackID = aStep->GetTrack()->GetTrackID();

  G4VPhysicalVolume *thePhysical = touchable->GetVolume();
  _hit_time = preStepPoint->GetGlobalTime();
  _hit_pos = preStepPoint->GetPosition();
  _hit_pdg = aTrack->GetDefinition()->GetPDGEncoding();
  _hit_volume = thePhysical->GetName();
  _hit_E = aStep->GetTotalEnergyDeposit();
  const G4VProcess *theProcess = postStepPoint->GetProcessDefinedStep();
  if (theProcess) {
    _hit_process_name = theProcess->GetProcessName();
  } else {
    _hit_process_name = "UserLimit";
  }
  //_hit_panel_id = thePhysical->GetCopyNo();
  //_hit_panel_id = preStepPoint->GetPhysicalVolume()->GetCopyNo();

  int barID = touchable->GetVolume(0)->GetCopyNo(); // scintPanel
  int layerID =
      touchable->GetVolume(1)
          ->GetCopyNo(); // TrackerLayerMom's placement (rotation index)
  int moduleID =
      touchable->GetVolume(2)->GetCopyNo(); // muTrackerBlockLV placement

  _hit_panel_id =
      moduleID * (fNOneModule * fNSciBars) + layerID * fNSciBars + barID;
  //_hit_panel_id = moduleID * 32 + layerID  * 8 + barID;

  GeoMuTrackerSDHit *trackerHit = new GeoMuTrackerSDHit();

  trackerHit->SetTime(_hit_time);
  trackerHit->SetPos(_hit_pos);
  trackerHit->SetEdep(_hit_E);
  trackerHit->SetPDG(_hit_pdg);
  trackerHit->SetID(_hit_panel_id);
  trackerHit->SetLogVName(_hit_volume);
  trackerHit->SetHitProcessName(_hit_process_name);

  _hitsCollection->insert(trackerHit);

  debug << "GeoMuTrackerSD::ProcessHits end." << newline;
  return true;
}

void GeoMuTrackerSD::EndOfEvent(G4HCofThisEvent * /*HCE*/) { ; }

} // namespace RAT
