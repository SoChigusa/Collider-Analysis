
/*
 * Since:  2018/09/17
 * Author: So Chigusa
 * Description: Read HepMC file and store information
 */

#include "hepmc_reader.h"

void copyParticle(Particle & p1, const Particle & p2) {
  p1.pID = p2.pID;
  p1.px = p2.px;
  p1.py = p2.py;
  p1.pz = p2.pz;
  p1.E = p2.E;
  p1.m = p2.m;
}

double invMass(const Particle & p1, const Particle & p2) {
  return sqrt((p1.E + p2.E) * (p1.E + p2.E)
	      - (p1.px + p2.px) * (p1.px + p2.px)
	      - (p1.py + p2.py) * (p1.py + p2.py)
	      - (p1.pz + p2.pz) * (p1.pz + p2.pz));
}

double momSq(const Particle & p1, const Particle & p2) {
  return (p1.px+p2.px)*(p1.px+p2.px)
    +(p1.py+p2.py)*(p1.py+p2.py)
    +(p1.pz+p2.pz)*(p1.pz+p2.pz);
}

HepMCReader::HepMCReader(const std::string &arg_ifname)
  : nev(-1), ifs(arg_ifname), isAnalyze(false), isAllStates(false), isAllFinalStates(false), isOutputLines(false) {
  std::cout << "Start reading " << arg_ifname << std::endl;
  // get first event number
  std::string tmp;
  while(nev == -1 && getline(ifs, tmp)) {
    analyzeLine(tmp);
  }
}

void HepMCReader::toParticle(Particle &arg_p, const int arg_pid, const std::vector<std::string> &arg_v) {
  arg_p.pID = arg_pid;
  arg_p.px = stod(arg_v[3]);
  arg_p.py = stod(arg_v[4]);
  arg_p.pz = stod(arg_v[5]);
  arg_p.E = stod(arg_v[6]);
  arg_p.m = stod(arg_v[7]);
  if(stod(arg_v[11]) != 0.) arg_p.Nsub = stod(arg_v[12]) / stod(arg_v[11]); // N-subjettiness for convenience
}

// analyze all lines
void HepMCReader::analyze() {
  std::string buf;
  fp.resize(0);
  dp.resize(0);
  vec_vtx.resize(bid.size());
  isAnalyze = true;
  while(getline(ifs, buf)) {
    analyzeLine(buf);
  }
}

void HepMCReader::analyzeLine(const std::string &arg_buf) {
  if(arg_buf == "") return;
  std::vector<std::string> v;
  split(v, arg_buf, ' ');
  
  // new event block starts
  if(v[0] == "E") {
    nev = stoi(v[1]);
    nv = 0;
    for(int i = 0; i < vec_vtx.size(); ++i) vec_vtx[i] = 0;
    if(isOutputLines) ofs << "E " << nev << std::endl;
    // if(nev % 1000 == 0) std::cout << nev << std::endl;
  }
  if(!isAnalyze) return;
  
  // particle info
  if(v[0] == "P") {
    int np = stoi(v[2]);

    // final state particle search
    if(fid.size() > 0) {

      // found final state particle
      auto itr = std::find(fid.begin(), fid.end(), np);
      size_t index = std::distance(fid.begin(), itr);
      if(index != fid.size()) {
	
	// see if next vertex exist (= final state or not)
	if(stoi(v[11]) == 0) {
	  // std::cout << nev << " " << arg_buf << std::endl;
	  if(isOutputLines) ofs << arg_buf << std::endl;
	  Particle pp;
	  toParticle(pp, np, v);
	  fp.push_back(pp);
	}
	
      }
    }

    // decaying particle search
    if(did.size() > 0) {

      // found decaying particle
      auto itr = std::find(did.begin(), did.end(), np);
      size_t index = std::distance(did.begin(), itr);
      if(index != did.size()) {
	
	// std::cout << nev << " " << arg_buf << std::endl;
	if(isOutputLines) ofs << arg_buf << std::endl;
	Particle pp;
	toParticle(pp, stoi(v[11]), v); // temporary conserve next vertex here
	dp.push_back(pp);

	/*
	 * if this corresponds to (dp = decaying particle)
	 * dp -> dp process, eliminate previous information
	 */
	for(auto i = dp.begin(); i != dp.end(); i) {
	  if((*i).pID == nv) i = dp.erase(i);
	  else i++;
	}
	
      }
    }

    // show all (final) state particles
    if(isAllStates || (isAllFinalStates && stoi(v[11]) == 0)) {
      // std::cout << nev << " " << arg_buf << std::endl;
      if(isOutputLines) ofs << arg_buf << std::endl;
      Particle pp;
      toParticle(pp, np, v);
      fp.push_back(pp);
    }

    // branch search
    if(bid.size() > 0) {
      
      // found decay particle
      auto itr = std::find(bid.begin(), bid.end(), np);
      size_t index = std::distance(bid.begin(), itr);
      if(index != bid.size()) vec_vtx[index] = stoi(v[11]);

      // output decay products
      for(int i = 0; i < vec_vtx.size(); ++i) {
	if(vec_vtx[i] > 0) {
	  if(isOutputLines) ofs << arg_buf << std::endl;
	  std::string key = std::to_string(i) + "_"
	    + std::to_string(vec_vtx[i]) + "_"
	    + std::to_string(np);
	  dbranch[key]++;
	  vec_vtx[i]++;
	}
      }
      
    }
  }

  // vertex info
  if(v[0] == "V") {
    for(int i = 0; i < vec_vtx.size(); ++i) if(vec_vtx[i] > 0) vec_vtx[i] = 0;
    nv = stoi(v[1]);

    // found decay particle
    if(bid.size() > 0) {
      auto itr = std::find(vec_vtx.begin(), vec_vtx.end(), nv);
      size_t index = std::distance(vec_vtx.begin(), itr);
      if(index != vec_vtx.size()) vec_vtx[index] = 1;
    }
  }
}

// analyze next event
void HepMCReader::analyzeNextEvent() {
  const int cev = nev;
  std::string buf;
  fp.resize(0);
  dp.resize(0);
  vec_vtx.resize(bid.size());
  isAnalyze = true;
  while(getline(ifs, buf)) {
    analyzeLine(buf);
    if(nev != cev) { isAnalyze = false; break; }
  }
  if(nev == cev) nev = -1; // end of file
}

// analysis controlled by arg_nev and isAnalyze
void HepMCReader::analyzeGivenNev(const int arg_nev) {
  std::string buf;
  fp.resize(0);
  dp.resize(0);
  vec_vtx.resize(bid.size());
  isAnalyze = (arg_nev == nev ? true : false);
  while(getline(ifs, buf)) {
    analyzeLine(buf);
    if(!isAnalyze && arg_nev == nev) isAnalyze = true;
    if(isAnalyze && arg_nev != nev) { isAnalyze = false; break; }
  }
}

void HepMCReader::split(std::vector<std::string> &v,
			const std::string &buf, char sep) {
  std::stringstream ss(buf);
  std::string mbuf;
  while(getline(ss, mbuf, sep)) { if(mbuf != "") v.push_back(mbuf); }
}

void HepMCReader::setOutputFile(const std::string &arg_fname) {
  ofs = std::ofstream(arg_fname.c_str());
  ofs << "E 0" << std::endl;
  flagOutputLines(true);
}
