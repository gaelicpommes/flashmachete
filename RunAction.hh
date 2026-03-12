
// include/RunAction.hh
#pragma once
#include "G4UserRunAction.hh"
#include <string>

class RunAction : public G4UserRunAction {
public:
  RunAction() = default;
  ~RunAction() override = default;

  void BeginOfRunAction(const G4Run*) override;
  void EndOfRunAction(const G4Run*) override;

  void SetOutPrefix(const std::string& p) { fPrefix = p; }
  const std::string& GetOutPrefix() const { return fPrefix; }

private:
  std::string fPrefix = "exitps"; // default output prefix
};
