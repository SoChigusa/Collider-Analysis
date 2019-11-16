
/*
 * Since:  2018/09/17
 * Author: So Chigusa
 * Description: Read HepMC file and store information
 */

#ifndef HEPMC_READER_H
#define HEPMC_READER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

struct Particle {
  int pID;
  double px;
  double py;
  double pz;
  double E;
  double m;
  double Nsub;
  double pT() const { return sqrt(px*px+py*py); }
  double p() const { return sqrt(px*px+py*py+pz*pz); }
  double y() const {
    return -0.5*log((E+pz)/(E-pz));
  }
  double eta() const {
    double t = pT() / pz;
    return -log((sqrt(t*t+1.)-1.)/fabs(t));
  }
  double phi() const {
    double ac = acos(px/pT());
    return py > 0. ? ac : 2.*M_PI-ac;
  }
};

void copyParticle(Particle & p1, const Particle & p2);
double invMass(const Particle & p1, const Particle & p2);
double momSq(const Particle & p1, const Particle & p2);

class HepMCReader {
 private:
  std::string ifn;
  std::ifstream ifs;
  std::ofstream ofs;
  bool isAnalyze;
  bool isAllStates;
  bool isAllFinalStates;
  bool isOutputLines;
  int nev, nv, np;
  std::vector<int> fid, did, bid;
  std::vector<Particle> fp, dp;
  std::vector<int> vec_vtx;
  std::unordered_map<std::string, int> dbranch;
  
 public:
  HepMCReader(const std::string &);
  void toParticle(Particle &, const int, const std::vector<std::string> &);
  void addFinalState(const int f) { fid.push_back(f); }
  void addDecayingState(const int d) { did.push_back(d); }
  void addDecayBranch(const int b) { bid.push_back(b); } // NOTE: only one branch for one decay particle in an event shown
  void analyze();
  void analyzeLine(const std::string &);
  void analyzeNextEvent();
  void analyzeGivenNev(const int);
  void split(std::vector<std::string> &, const std::string &, char);
  void setOutputFile(const std::string &);
  void flagAllStates(bool f) { isAllStates = f; }
  void flagAllFinalStates(bool f) { isAllFinalStates = f; }
  void flagOutputLines(bool f) { isOutputLines = f; }
  int displayNev() { return nev; }
  void displayFinalParticles(std::vector<Particle> & ffpp) { ffpp = fp; }
  void displayDecayingParticles(std::vector<Particle> & ddpp) { ddpp = dp; }
  void displayDBranch(std::unordered_map<std::string, int> & db) { db = dbranch; }
};

#endif
