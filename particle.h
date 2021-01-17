/*
 * Since:  2018/12/07
 * Author: So Chigusa
 * Description: Particle struct common for LHE and hepmc operation
 */
#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include <vector>

inline double sqr(double x) { return x*x; }

struct Particle {
  int pID;
  double px;
  double py;
  double pz;
  double E;
  double m;
  double Nsub; // temporal
  void set(double a, double b, double c, double e) {
    px = a; py = b; pz = c; E = e;
    m = sqrt(E*E-px*px-py*py-pz*pz);
  }
  double pT() const { return sqrt(px*px+py*py); }
  double p() const { return sqrt(px*px+py*py+pz*pz); }
  double y() const {
    return -0.5*log((E+pz)/(E-pz));
  }
  double cos_theta() const {
    return pz / sqrt(px*px + py*py + pz*pz);
  }
  double sin_theta() const {
    return pT() / sqrt(px*px + py*py + pz*pz);
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

// invariant mass of cluster of momenta
double invariant_mass(const std::vector<Particle> &);

// transverse mass of lepton plus missing
double transverse_mass(const Particle &, const Particle &);

#endif
