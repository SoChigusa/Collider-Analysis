/*
 * Since:  2022/06/22
 * Author: So Chigusa
 * Description: Extract information of final state particles from MadGraph output LHE
 */

#include <fstream>
#include <iomanip>

#include "lhe_reader.h"

using namespace std;

int main()
{

  std::cout << std::scientific << std::setprecision(6);

  try
  {

    // output data file
    char ofname[] = "<output file name>";
    std::ofstream ofs(ofname);
    ofs << scientific << setprecision(6);

    // header
    ofs << "pID1\t"
        << "px1\t"
        << "py1\t"
        << "pz1\t"
        << "E1\t"
        << "..." << endl;

    // input LHE
    LHEReader myLHE("<path to input LHE file>");
    int N = myLHE.displayN();

    // extract event info
    Event ev;
    for (int n = 0; n < N; ++n)
    {
      myLHE.displayEvent(n, ev);

      // only final state particles
      for (auto p : ev.p)
      {
        if (p.status >= 1)
        {
          ofs << p.pID << "\t";
          ofs << p.px << "\t";
          ofs << p.py << "\t";
          ofs << p.pz << "\t";
          ofs << p.E << "\t";
        }
      }
      ofs << "\n";
    }
  }
  catch (const std::string &a)
  {
    std::cerr << a << std::endl;
    return -1;
  }
  catch (const char *a)
  {
    std::cerr << a << std::endl;
    return -1;
  }
  catch (...)
  {
    std::cerr << "Unknown type of exception caught.\n";
    return -1;
  }

  return 0;
}
