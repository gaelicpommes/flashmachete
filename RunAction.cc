//src/RunAction.cc
#include "RunAction.hh"
#include "Analysis.hh"
#include "G4Run.hh"
#include "G4ios.hh"

void RunAction::BeginOfRunAction(const G4Run*) {
  Analysis::Instance()->Configure(
      /*rMax_mm*/ 60.0, /*nR*/ 120,
      /*eMax_MeV*/ 12.0, /*nE*/ 240,
      /*thMax_deg*/ 30.0, /*nTh*/ 120);
  Analysis::Instance()->Reset();
}

void RunAction::EndOfRunAction(const G4Run* run) {
  // Append runID so outputs don't overwrite each time
  const int runID = run ? run->GetRunID() : 0;
  const std::string out = fPrefix + "_run" + std::to_string(runID);

  Analysis::Instance()->WriteCSV(out);

  G4cout << "[Analysis] Wrote exit plane histograms with prefix: "
         << out.c_str() << G4endl;
}
