/*
 * Since:  2018/12/07
 * Author: So Chigusa
 * Description: Particle struct common for LHE and hepmc operation
 */

#include "particle.h"

// invariant mass of cluster of momenta
double invariant_mass(const std::vector<Particle> &arg_vm) {
  double px = 0., py = 0., pz = 0., E = 0.;
  for(auto x : arg_vm) {
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

