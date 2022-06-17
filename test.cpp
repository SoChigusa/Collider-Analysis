
/** \file main.cpp
    - Author:      So Chigusa
    - Since:       2019/11/06
    \brief Test cpp file for Collider-Analysis.
*/

#include "particle.h"
#include "lhe_reader.h"
#include "hepmc_reader.h"

int main() {

  const std::time_t start = std::time(nullptr);
  std::cout << std::scientific << std::setprecision(6);

try {



 }
 catch(const std::string & a) { std::cerr << a; return -1; }
 catch(const char * a) { std::cerr << a; return -1; }
 catch(...) { std::cerr << "Unknown type of exception caught.\n"; return -1; }
  
 std::cout << "Finish at " << std::time(nullptr)-start << std::endl;
 return 0;
}

// full 1-loop    6.430230e+04 1.472631e+04 2.228449e+04 1.656715e+09 3.014007e+09 9.490154e+08 9.232160e+08 9.135756e+08 1.887337e+09 9.682687e+08 3.615255e+09
// partial 2-loop 6.446765e+04 1.469194e+04 2.220874e+04 1.668996e+09 3.079618e+09 9.190144e+08 9.146289e+08 9.058658e+08 1.889630e+09 9.701955e+08 3.629820e+09
// full 2-loop    6.446765e+04 1.469194e+04 2.220874e+04 1.670296e+09 3.081840e+09 9.193577e+08 9.138525e+08 9.072104e+08 1.878860e+09 9.488101e+08 3.647451e+09
