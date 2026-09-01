#include <RAT/GLG4StringUtil.hh>
#include <RAT/Log.hh>
#include <RAT/PosGen_Muon.hh>
#include <Randomize.hh>
#include <sstream>

namespace RAT {

PosGen_Muon::PosGen_Muon(const char *arg_dbname)
    : GLG4PosGen(arg_dbname), fLength(0.0), fWidth(0.0), fHeight(0.0),
      fCenter(0.0, 0.0, 0.0) {}

void PosGen_Muon::GeneratePosition(G4ThreeVector &argResult) {

  argResult =
      G4ThreeVector(2 * fLength * G4UnifromRand() - fLength + fCenter.x(),
                    2 * fWidth * G4UnifromRand() - fWidth + fCenter.y(),
                    fHeight + fCenter.z());
}

void PosGen_Muon::SetState(G4String newValues) {
  newValues = util_strip_default(newValues);
  if (newValues.length() == 0) {
    // print help and current state
    info << "Current state of this GLG4PosGen_Muon:" << newline << " \""
         << GetState() << "\"" << newline << newline;
    info << "Format of argument to PosGen_Muon::SetState: " << newline
         << " \"l_mm w_mm h_mm xc_mm yc_mm zc_mm\"" << newline;
    return;
  }

  std::istringstream is(newValues.c_str());

  // set position
  G4double l, w, h, xc, yc, zc;
  is >> l >> w >> h >> xc >> yc >> zc;
  if (is.fail()) {
    warn << "PosGen_Muon::SetState: Could not parse six doubles from input "
            "string"
         << newline;
    return;
  }

  fCenter = G4ThreeVector(xc, yc, zc);
  fLength = l;
  fWidth = w;
  fHeight = h;
}

G4String PosGen_Muon::GetState() const {
  return util_dformat("%ld\t%ld\t%ld -> %ld\t%ld\t%ld", fLength, fWidth,
                      fHeight, fCenter.x(), fCenter.y(), fCenter.z());
}

} // namespace RAT
