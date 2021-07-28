/*
 * Since:  2018/12/07
 * Author: So Chigusa
 * Description: Particle struct common for LHE and hepmc operation
 */
#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include <vector>

inline double sqr(double x) { return x * x; }

struct Particle {
  int pID;
  double px;
  double py;
  double pz;
  double E;
  double m;
  double Nsub; // temporal
  void set(double a, double b, double c, double e) {
    px = a;
    py = b;
    pz = c;
    E = e;
    m = sqrt(E * E - px * px - py * py - pz * pz);
  }
  double pT() const { return sqrt(px * px + py * py); }
  double p() const { return sqrt(px * px + py * py + pz * pz); }
  double y() const { return -0.5 * log((E + pz) / (E - pz)); }
  double cos_theta() const { return pz / sqrt(px * px + py * py + pz * pz); }
  double sin_theta() const { return pT() / sqrt(px * px + py * py + pz * pz); }
  double eta() const {
    double t = pT() / pz;
    return -log((sqrt(t * t + 1.) - 1.) / fabs(t));
  }
  double phi() const {
    double ac = acos(px / pT());
    return py > 0. ? ac : 2. * M_PI - ac;
  }
};

// invariant mass of cluster of momenta
double invariant_mass(const std::vector<Particle> &arg_vm) {
  double px = 0., py = 0., pz = 0., E = 0.;
  for (auto x : arg_vm) {
    px += x.px;
    py += x.py;
    pz += x.pz;
    E += x.E;
  }
  const double imsq = sqr(E) - sqr(px) - sqr(py) - sqr(pz);
  return imsq >= 0. ? sqrt(imsq) : 0.;
}

// transverse mass of lepton plus missing
double transverse_mass(const Particle &arg_pl, const Particle &arg_met) {
  const double dphi = arg_pl.phi() - arg_met.phi();
  const double imsq = 2. * arg_pl.pT() * arg_met.pT() * (1. - cos(dphi));
  return imsq >= 0. ? sqrt(imsq) : 0.;
}

#endif
