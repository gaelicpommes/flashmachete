//src/Analysis.cc
#include "Analysis.hh"
#include <fstream>
#include <algorithm>
#include <cmath>

Analysis* Analysis::fInstance = nullptr;

Analysis* Analysis::Instance() {
  if (!fInstance) fInstance = new Analysis();
  return fInstance;
}



void Analysis::Configure(double rMax_mm, int nR,
                         double eMax_MeV, int nE,
                         double thMax_deg, int nTh) {
  std::lock_guard<std::mutex> lock(fMutex);
  fRmax=rMax_mm; fNR=nR;
  fEmax=eMax_MeV; fNE=nE;
  fThmax=thMax_deg; fNTh=nTh;

  hR.assign(fNR,0);
  hE.assign(fNE,0);
  hTh.assign(fNTh,0);
}

void Analysis::Reset() {
  std::lock_guard<std::mutex> lock(fMutex);
  std::fill(hR.begin(),hR.end(),0);
  std::fill(hE.begin(),hE.end(),0);
  std::fill(hTh.begin(),hTh.end(),0);
}

int Analysis::BinIndex(double x, double xmax, int n) const {
  if (x < 0) return -1;
  if (x >= xmax) return -1;
  int i = (int)std::floor((x/xmax)*n);
  if (i < 0 || i >= n) return -1;
  return i;
}

void Analysis::FillR(double r_mm) {
  std::lock_guard<std::mutex> lock(fMutex);
  int i = BinIndex(r_mm,fRmax,fNR);
  if (i>=0) hR[i]++;
}

void Analysis::FillE(double e_MeV) {
  std::lock_guard<std::mutex> lock(fMutex);
  int i = BinIndex(e_MeV,fEmax,fNE);
  if (i>=0) hE[i]++;
}

void Analysis::FillTheta(double th_deg) {
  std::lock_guard<std::mutex> lock(fMutex);
  int i = BinIndex(th_deg,fThmax,fNTh);
  if (i>=0) hTh[i]++;
}

void Analysis::WriteCSV(const std::string& /*outPrefix*/) const {
  std::lock_guard<std::mutex> lock(fMutex);

  // radial fluence at exit
  {
    std::ofstream f("exit_r_fluence.csv", std::ios::trunc);
    f << "r_mm,count\n";
    for (int i=0;i<fNR;i++){
      double x = (i+0.5)*fRmax/fNR;
      f << x << "," << hR[i] << "\n";
    }
  }

  // energy spectrum at exit
  {
    std::ofstream f("exit_energy.csv", std::ios::trunc);
    f << "E_MeV,count\n";
    for (int i=0;i<fNE;i++){
      double x = (i+0.5)*fEmax/fNE;
      f << x << "," << hE[i] << "\n";
    }
  }

  // angular distribution at exit
  {
    std::ofstream f("exit_theta.csv", std::ios::trunc);
    f << "theta_deg,count\n";
    for (int i=0;i<fNTh;i++){
      double x = (i+0.5)*fThmax/fNTh;
      f << x << "," << hTh[i] << "\n";
    }
  }
}