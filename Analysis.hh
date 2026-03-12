//include/Analysis.hh
#pragma once
#include <vector>
#include <mutex>
#include <string>

class Analysis {
public:
  static Analysis* Instance();

  // Call once at start of run (or before beamOn)
  void Configure(double rMax_mm, int nR,
                 double eMax_MeV, int nE,
                 double thMax_deg, int nTh);

  void Reset(); // clear histograms

  void FillR(double r_mm);
  void FillE(double e_MeV);
  void FillTheta(double th_deg);

  void WriteCSV(const std::string& outPrefix) const;

private:
  Analysis() = default;
  static Analysis* fInstance;

  mutable std::mutex fMutex;

  double fRmax=60.0; int fNR=60;
  double fEmax=12.0; int fNE=120;
  double fThmax=30.0; int fNTh=60;

  std::vector<long long> hR, hE, hTh;

  int BinIndex(double x, double xmax, int n) const;
};
