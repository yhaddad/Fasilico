#ifndef utils_h
#define utils_h 1



char cred[8]     = {0x1b,'[','1',';','3','1','m',0};
char cblue[8]    = {0x1b,'[','1',';','3','4','m',0};
char cnor[8]     = {0x1b,'[','0',';','3','9','m',0};


bool is_neutrino(MCParticle* p){
  if(p->getPDG() == 12 || p->getPDG() == 14 || p->getPDG() == 16 || p->getPDG() == 18)
    return true;
  else 
    return false;
}
bool is_muon(MCParticle* p){
  if(p->getPDG() == 13 || p->getPDG() == -13 )
    return true;
  else 
    return false;
}

bool is_photon(MCParticle* p){
  if(p->getPDG() == 22)
    return true;
  else
    return false;
}

bool is_electron(MCParticle* p){
  if(fabs(p->getPDG()) == 11 )
    return true;
  else
    return false;
}

double get_m2  (double *p)  { 
  return p[3]*p[3] - (p[0]*p[0] + p[1]*p[1] + p[2]*p[2]);
}

double get_recoil_m(double *p) { 
  return sqrt(250.0*250.0 + get_m2(p) - 2*p[3]*250.0);
}

//double get_recoil_M(TLorentzVector p) { 
//  return sqrt(250.0*250.0 + p.M2() - 2*p.E()*250.0);
//}

#endif
