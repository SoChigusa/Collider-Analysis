/*
 * Since:  2018/06/15
 * Author: So Chigusa
 * Description: Read LHE file and store information
 */

#ifndef LHE_READER_H
#define LHE_READER_H

#include "particle.h"
#include <fstream>
#include <iostream>
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
  void displayEventStr(std::string &buf, const int n) { buf = vec_buf[n + 1]; }
  void displayFooter(std::string &buf) { buf = vec_buf.back(); }
};

// read aMC@NLO LHE file
LHEReader::LHEReader(std::string fname) {
  std::ifstream ifs(fname);
  std::string buf_ne, buf_pb;
  findStr(ifs, buf_ne, "nevents");
  findStr(ifs, buf_pb, "<init>");
  nextNline(ifs, buf_pb, 2);

  std::vector<std::string> v;
  v = split(buf_ne, '=');
  nevent = stoi(v[0]);
  v = split(buf_pb, ' ');
  xsec = stod(v[0]);
  std::cout << fname << ": # of events = " << nevent << ", sigma = " << xsec
            << " pb." << std::endl;

  ev.resize(nevent);
  int n = 0;
  std::string buf_ev;
  while (findStr(ifs, buf_ev, "<event>"))
    saveEvent(ifs, n);

  std::ifstream ifs_str(fname);
  saveStr(ifs_str, "</init>");
  while (saveStr(ifs_str, "</event>")) {
  }
}

int LHEReader::findStr(std::ifstream &ifs, std::string &buf,
                       const std::string &f) {
  getline(ifs, buf);
  while (buf.find(f) == std::string::npos) {
    if (!getline(ifs, buf)) {
      return 0;
    }
  }
  return 1;
}

void LHEReader::nextNline(std::ifstream &ifs, std::string &buf, const int n) {
  for (int i = 0; i < n; i++) {
    getline(ifs, buf);
  }
}

std::vector<std::string> LHEReader::split(const std::string &buf, char sep) {
  std::vector<std::string> v;
  std::stringstream ss(buf);
  std::string mbuf;
  while (getline(ss, mbuf, sep)) {
    if (mbuf != "")
      v.push_back(mbuf);
  }
  return v;
}

void LHEReader::saveEvent(std::ifstream &ifs, int &n) {
  static std::vector<std::string> sp_head, sp_pinfo;
  std::string head, pinfo;

  getline(ifs, head);
  sp_head = split(head, ' ');
  double nparticle = stod(sp_head[0]);

  Event event;
  Particle p;
  for (int i = 0; i < nparticle; i++) {
    getline(ifs, pinfo);
    sp_pinfo = split(pinfo, ' ');
    p.pID = stoi(sp_pinfo[0]);
    p.E = stod(sp_pinfo[9]);
    p.px = stod(sp_pinfo[6]);
    p.py = stod(sp_pinfo[7]);
    p.pz = stod(sp_pinfo[8]);
    p.m = stod(sp_pinfo[9]);
    event.p.push_back(p);
  }

  ev[n] = event;
  n++;
}

int LHEReader::saveStr(std::ifstream &arg_ifs, const std::string &arg_f) {
  int res = 1;
  std::stringstream ss;
  std::string buf;
  getline(arg_ifs, buf);
  ss << buf << std::endl;
  while (buf.find(arg_f) == std::string::npos) {
    if (!getline(arg_ifs, buf)) {
      ss << buf << std::endl;
      res = 0;
      break;
    }
    ss << buf << std::endl;
  }
  vec_buf.push_back(ss.str());
  return res;
}

#endif
