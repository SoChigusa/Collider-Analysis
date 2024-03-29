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
  int status; // -1 for initial, 1 for final
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
  double y() const { return 0.5 * log((E + pz) / (E - pz)); }
  double cos_theta() const { return pz / sqrt(px * px + py * py + pz * pz); }
  double sin_theta() const { return pT() / sqrt(px * px + py * py + pz * pz); }
  double eta() const {
    double th = atan2(pT(), pz); // in (y,x) ordering
    return -log(tan(0.5 * th));
  }
  double phi() const { // returns val in (-pi, pi)
    return atan2(py, px);
  }
  double dot3D(Particle arg_p) const {
    return px * arg_p.px + py * arg_p.py + pz * arg_p.pz;
  }
  double cos_theta(Particle arg_p) const {
    return dot3D(arg_p) / (p() * arg_p.p());
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
