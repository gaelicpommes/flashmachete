// include/BeamlineGeometry.hh
#pragma once

#include "globals.hh"
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

struct BeamlineHandles {
  // Useful z landmarks (all in global coords)
  G4double zWindow = 0.0;
  G4double zFoil1  = 0.0;
  G4double zFoil4  = 0.0;
  G4double zCollCenter = 0.0;
  G4double zCollExit   = 0.0;
  G4double zAppEntrance = 0.0;
  G4double zAppExit     = 0.0;
  //G4double zExitPlane = 0.0;

  // Optionally keep pointers if you want later
  G4LogicalVolume* applicatorLV = nullptr;
};

class BeamlineGeometry {
public:
  // Builds: eWindow reference disk @ z=0, 4 Al foils, simple collimator body-with-hole,
  // and the 10 cm PMMA applicator, placed flush to the collimator exit.
  static BeamlineHandles Build10cmBeamline(G4LogicalVolume* worldLV);
};
