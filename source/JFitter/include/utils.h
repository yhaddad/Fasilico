#ifndef utils_h
#define utils_h 1


namespace utils{
  const char cred [8]     = {0x1b,'[','1',';','3','1','m',0};
  const char cblue[8]     = {0x1b,'[','1',';','3','4','m',0};
  const char cnor [8]     = {0x1b,'[','0',';','3','9','m',0};
  
  inline bool is_neutrino(MCParticle* p){
    if(p->getPDG() == 12 || p->getPDG() == 14 || p->getPDG() == 16 || p->getPDG() == 18)
      return true;
    else 
      return false;
  }
  inline bool is_muon(MCParticle* p){
    if(p->getPDG() == 13 || p->getPDG() == -13 )
      return true;
    else 
      return false;
  }
  
  inline bool is_photon(MCParticle* p){
    if(p->getPDG() == 22)
      return true;
    else
      return false;
  }
  
  inline bool is_electron(MCParticle* p){
    if(fabs(p->getPDG()) == 11 )
      return true;
    else
      return false;
  }
  
  /** @brief return invariante mass of p = {px,py,pz,e}
   * @param p = {px,py,pz,e}
   */
  inline double get_m2  (double *p)  { 
    return p[3]*p[3] - (p[0]*p[0] + p[1]*p[1] + p[2]*p[2]);
  }
  /**@brief return the recoil mass
   * @param p = {px,py,pz,e} ,  center mass energy \f$ \sqrt{s} \f$ 
   */
  inline double recoilMass(double *p, double sqrts) { 
    return sqrt( sqrts*sqrts + get_m2(p) - 2*p[3]*sqrts);
  }
  
  inline double recoilMass(CLHEP::HepLorentzVector p, double sqrts){
    return sqrt( sqrts*sqrts + p.m2() - 2*p.e()*sqrts);
  }
  
  //double get_recoil_M(TLorentzVector p) { 
  //  return sqrt(250.0*250.0 + p.M2() - 2*p.E()*250.0);
  //}
}
#endif
