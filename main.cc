// main.cc  (simple, robust UI+macro handling)
// main.cc
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "G4ScoringManager.hh"

int main(int argc, char** argv)
{
  // Force serial so UI commands like /gps are available in the UI session
  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

  G4ScoringManager::GetScoringManager();

  runManager->SetUserInitialization(new DetectorConstruction());

  auto* physics = new FTFP_BERT;
  physics->ReplacePhysics(new G4EmStandardPhysics_option4());
  runManager->SetUserInitialization(physics);

  runManager->SetUserInitialization(new ActionInitialization());

  auto* visManager = new G4VisExecutive();
  visManager->Initialize();

  auto* uiManager = G4UImanager::GetUIpointer();

  if (argc > 1) {
    uiManager->ApplyCommand("/control/execute " + G4String(argv[1]));
  } else {
    auto* ui = new G4UIExecutive(argc, argv);
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;
  return 0;
}
