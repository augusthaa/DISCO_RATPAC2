{
name: "GEO",
run_range: [0,0],
index: "disco_world",
mother: "",
type: "box",
size: [1000.0, 1000.0, 1000.0],
material: "G4_AIR",
invisible: 1,
}

{
name:"GEO",
index: "MuonTracker",
run_range: [0,0], 
type: "discoMuonTracker",
mother: "disco_world",
worldMaterial: "G4_AIR",
fiberDiameter: 0.8,
fiberLength: 190.0,
ScintPanelLenX: 190.0,
ScintPanelLenY: 19.0,
ScintPanelLenZ: 15.0,
ModuleDistance: 40.0,
nFibers: 10,
nSciBars: 10,
nOneModule: 4,
nModules: 2,
enable_mu_tracker: 1,
trackerPosition: [0.0,0.0,500.0],
}

{
name: "GEO",
index: "TestCell",
run_range: [0,0],
type: "discoTestCell",
mother: "disco_world",
TestCellHeight: 200.0,
CellDiameter: 300.0,
CellThickness: 10.0,
AcrylicThickness: 10.0,
TopCoverThickness: 10.0,
DetMediumHeight: 100.0,
detection_medium: "water",
enable_dome: 1,
TestCellPosition:[0.0,0.0,0.0],
}
//

//PMT housnig and holders
{
name: "GEO",
index: "PMTHousing",
run_range: [0,0],
type: "discoPMTHousing",
mother: "disco_world",
worldMaterial: "G4_AIR",
rmin_adit: 12.7,
rmax_adit: 14.7,
rmin_hamamatsu: 14.25,
rmax_hamamatsu: 16.25,
PMThousingPosition: [0.0,0.0,-500.0]
}

//pmts
{
name: "GEO",
index: "inner_ring_pmts",
enable: 1,
run_range: [0, 0],
mother: "disco_world",
type: "pmtarray",
pmt_model: "hamamatsu",
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner",
efficiency_correction: 0.70000,
pos_table: "PMTINFO_hamamatsu_pmts",
orientation: "manual",
color: [0.3,0.5, 0.0, 0.7],
}

{
name: "GEO",
index: "outer_ring_pmts",
enable: 1,
run_range: [0, 0],
mother: "disco_world",
type: "pmtarray",
pmt_model: "adit",
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner",
efficiency_correction: 0.70000,
pos_table: "PMTINFO_adit_pmts",
orientation: "manual",
color: [0.3,0.5, 0.0, 0.7],
}

