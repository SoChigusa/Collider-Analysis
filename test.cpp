
/** \file main.cpp
    - Author:      So Chigusa
    - Since:       2019/11/06
    \brief Test cpp file for Collider-Analysis.
*/

#include "particle.h"
#include "lhe_reader.h"
// #include "hepmc_reader.h"

int main() {

  const std::time_t start = std::time(nullptr);
  std::cout << std::scientific << std::setprecision(6);

  try {

    // LHE reader example
    char ifname[] = "<path-to-LHE-file>";
    LHEReader myLHEReader = LHEReader(ifname);
    std::cout << "Total number of events: " << myLHEReader.displayN() << std::endl;
    std::cout << "Cross section: " << myLHEReader.displayCrossSection() << " pb" << std::endl;

    Event ev;
    myLHEReader.displayEvent(0, ev);
    for(auto p : ev.p) {
      std::cout << "pID = " << p.pID << ",\t p = (" << p.E << ", " << p.px << ", " << p.py << ", " << p.pz << ")" << std::endl;
    }

  }
  catch(const std::string & a) { std::cerr << a; return -1; }
  catch(const char * a) { std::cerr << a; return -1; }
  catch(...) { std::cerr << "Unknown type of exception caught.\n"; return -1; }
  
  std::cout << "Finish at " << std::time(nullptr)-start << std::endl;
  return 0;
}
