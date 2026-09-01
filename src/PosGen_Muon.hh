#ifndef __RAT_PosGen_Muon__
#define __RAT_PosGen_Muon__

#include <RAT/GLG4PosGen.hh>

namespace RAT {

class PosGen_Muon : public GLG4PosGen {
public:
  /** Create a new position generator.
   *  Generate points uniformly distributed in a rectange of length fLength and
   * width fWidth at height fHeight centered around fCenter The default position
   * is 0,0,0 and center at (0,0,0), i.e. a point at the origin.
   */
  PosGen_Muon(const char *arg_dbname = "rectangle");

  /** Generate a random position on the rectange */
  virtual void GeneratePosition(G4ThreeVector &argResult);

  /** Set the coordinates
   *
   *  @param[in] newValues   x1 y1 z1 x2 y2 z2 (in mm)
   */
  void SetState(G4String newValues);

  /** Return state as string in same format as SetState() */
  G4String GetState() const;

protected:
  G4ThreeVector fCenter;
  G4double fWidth;
  G4double fLength;
  G4double fHeight;
};

} // namespace RAT

#endif
