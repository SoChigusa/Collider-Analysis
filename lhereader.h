/*
 * Since:  2018/06/15
 * Author: So Chigusa
 * Description: Read LHE file and store information
 */

#include "particle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Event {
  std::vector<Particle> p;
};

class LHEReader {
 private:
  int nevent;
  double xsec;
  std::vector<std::string> vec_buf;
  std::vector<Event> ev;

 public:
  LHEReader(std::string);
  int findStr(std::ifstream &, std::string &, const std::string &);
  void nextNline(std::ifstream &, std::string &, const int);
  std::vector<std::string> split(const std::string &, char);
  void saveEvent(std::ifstream &, int &);
  int saveStr(std::ifstream &arg_ifs, const std::string &arg_f);
  
  int displayN() { return nevent; }
  double displayCrossSection() { return xsec; }
  Event displayEvent(const int n) { return ev[n]; }
  void displayHeader(std::string &buf) { buf = vec_buf[0]; }
  void displayEventStr(std::string &buf, const int n) { buf = vec_buf[n+1]; }
  void displayFooter(std::string &buf) { buf = vec_buf.back(); }
};

