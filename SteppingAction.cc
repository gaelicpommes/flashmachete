#include "SteppingAction.hh"
#include "Analysis.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"

#include <cmath>
#include <algorithm>

namespace {
  inline double Clamp(double x, double lo, double hi) {
    return std::max(lo, std::min(hi, x));
  }
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!step) return;

  auto prePV  = step->GetPreStepPoint()->GetPhysicalVolume();
  auto postPV = step->GetPostStepPoint()->GetPhysicalVolume();
  if (!prePV || !postPV) return;

  const auto& preName  = prePV->GetName();
  const auto& postName = postPV->GetName();

  auto* track = step->GetTrack();
  if (!track) return;

  // Score all electrons (not just primaries) — this is what shapes the PDD
  if (track->GetDefinition()->GetParticleName() != "e-") return;

  const auto postStatus = step->GetPostStepPoint()->GetStepStatus();

  // Score phase-space at phantom entrance (robust reference plane)
  const bool enteringWater =
      (preName != "WaterPhantomPV") &&
      (postName == "WaterPhantomPV") &&
      (postStatus == fGeomBoundary);

  if (!enteringWater) return;

  const auto pos = step->GetPostStepPoint()->GetPosition();
  const double r_mm = std::sqrt(pos.x()*pos.x() + pos.y()*pos.y()) / mm;

  const double e_MeV = step->GetPostStepPoint()->GetKineticEnergy() / MeV;

  const auto dir = step->GetPostStepPoint()->GetMomentumDirection();
  const double cosz = Clamp((double)dir.z(), -1.0, 1.0);
  const double theta_deg = std::acos(cosz) * 180.0 / CLHEP::pi;

  Analysis::Instance()->FillR(r_mm);
  Analysis::Instance()->FillE(e_MeV);
  Analysis::Instance()->FillTheta(theta_deg);
}
