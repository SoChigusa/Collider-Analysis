# Collider analysis code set

This is a set of c++ classes (and structs) that can be used to extract information from the MadGraph / Pythia8 output files.

## What's inside

- particle.h  
  Defines the `Particle` struct in which extracted information of each initial and final state particle is stored.
- lhe_reader.h  
  Defines the `LHEReader` class that is used to extract information from a `.lhe` file obtained as the MadGraph output.
- hepmc_reader.h  
  Defines the `HepMCReader` class that is used to extract information from a `.hepmc` file obtained as the Pythia8 output.

## `Particle` struct

The `Particle` struct contains the following information

- `pID`  
  Particle ID defined [here](https://pdg.lbl.gov/2019/reviews/rpp2019-rev-monte-carlo-numbering.pdf)
- `status`  
  -1 (1) for an initial (final) state particle and 2 for an initial state radiation
- `px`  
  x component of the momentum
- `py`  
  y component of the momentum
- `pz`  
  z component of the momentum
- `E`  
  Energy
- `m`  
  Mass

The struct also includes functions to evaluate some of the frequently-used kinematic variables.

- `pT()`  
  Transverse momentum
- `p()`  
  Absolute 3-momentum 
- `y()`  
  Rapidity
- `eta()`  
  Pseudo-rapidity
- `cos_theta()` and `sin_theta()`  
  Angle between the momentum and the beam axis
- `phi()`  
  Azimuthal angle

By combining with another `Particle` struct, it can also calculate the followings:

- `dot3D(Particle)`  
  Inner product
- `cos_theta(Particle)`  
  Angle between two 3-momenta

## Fundamental usage of the `LHEReader` class

In order to read a `.lhe` file, simply execute

``` c++
LHEReader myLHEReader = LHEReader("path-to-LHE-file");
```

This code stores information of all events in the `.lhe` file in the `MyLHEReader` object.
Some information of the `.lhe` file is available via

- `displayN()`  
  Total number of events $N$
- `displayCrossSection()`  
  Cross section

To access the $n$-th event ($0\leq n<N$), you should do

``` c++
Event ev;
myLHEReader.diplay(n, ev);
```

The `Event` struct is defined as

``` c++
struct Event {
  std::vector<Particle> p;
};
```

and the vector `p` is a list of `Particle` structs that contains information of all initial and final state particles of the $n$-th event.

## Examples

There are several example source codes in the `examples` directory.

- extract_info_from_LHE.dat  
  Reads a LHE file and extract particle IDs and 4-momenta of all the final state particles.