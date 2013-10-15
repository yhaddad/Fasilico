#include "HistBooker.h"

#include <TROOT.h>
#include <TStyle.h>
#include <algorithm>

HistBooker* gHistBooker;

HistBooker* HistBooker::_me = 0;
HistBooker* HistBooker::instance()
{
  if( _me == 0 )
    _me = new HistBooker;
  return _me;
}

HistBooker* HistBooker::global()
{
  if( gHistBooker == 0 )
    gHistBooker = new HistBooker;
  return gHistBooker;
}

HistBooker::HistBooker()
{
  std::cout << " ================================" << std::endl
	    << " = Histogram booker version 2.0 =" << std::endl
	    << " =         Y.Haddad             =" << std::endl
	    << " ================================" << std::endl;
  
  _map_h1.clear();
  _map_h2.clear();
  _map_h3.clear();
  
  //_auto_rebin = false;
}

//HistBooker::h1_auto_rebin(bool opt)
//{
//  if(opt == true )
//    { 
//      _h1_option = TH1::kCanRebin;
//      _auto_rebin = true;
//    }
//}


TH1* HistBooker::book_h1(std::string name,int nbin,double xmin,double xmax)
{
  std::vector<std::string> name_title;
  name_title = split(name,':');
  std::string sname = name_title[0]; 
  std::string title = name_title[1]; 
  
  // std::cout << "name  == " << name.c_str()  << std::endl;
  // std::cout << "sname == " << sname.c_str() << std::endl;
  // std::cout << "title == " << title.c_str() << std::endl;
  
  if(_map_h1.find(sname)!=_map_h1.end()){
    return _map_h1[sname]; 
  }else{
    TH1F* h = new TH1F(sname.c_str(),title.c_str(),nbin,xmin,xmax);
    h->SetDirectory(0);
    std::pair<std::string, TH1* > pr(sname,h);
    _map_h1.insert(pr);
    std::cout << sname.c_str() <<" [ booked ... ]" <<std::endl;
    return h;
  }
}

TH2* HistBooker::book_h2(std::string name,
			 int nbinx,double xmin,double xmax,
			 int nbiny,double ymin,double ymax)
{
  std::vector<std::string> name_title;
  name_title = split(name,':');
  std::string sname = name_title[0]; 
  std::string title = name_title[1]; 
  
  if(_map_h2.find(sname)!=_map_h2.end()){
    return _map_h2[sname]; 
  }else{
    TH2F* h = new TH2F(sname.c_str(),title.c_str(),nbinx,xmin,xmax,nbiny,ymin,ymax);
    h->SetDirectory(0);
    std::pair<std::string, TH2* > pr(sname,h);
    _map_h2.insert(pr);
    std::cout << sname.c_str() <<" [ booked ... ]" <<std::endl;
    return h;
  }
}

TH3* HistBooker::book_h3(std::string name,
			 int nbinx,double xmin,double xmax,
			 int nbiny,double ymin,double ymax,
			 int nbinz,double zmin,double zmax)
{
  std::vector<std::string> name_title;
  name_title = split(name,':');
  std::string sname = name_title[0]; 
  std::string title = name_title[1]; 
  
  if(_map_h3.find(sname)!=_map_h3.end()){
    return _map_h3[sname]; 
  }else{
    TH3F* h = new TH3F(sname.c_str(),title.c_str(),nbinx,xmin,xmax,nbiny,ymin,ymax,nbinz,zmin,zmax);
    h->SetDirectory(0);
    std::pair<std::string, TH3* > pr(sname,h);
    _map_h3.insert(pr);
    std::cout << sname.c_str() <<" [ booked ... ]" <<std::endl;
    return h;
  }
}
TH1* HistBooker::get_h1(std::string name)
{ 
  if(_map_h1.find(name)!=_map_h1.end()) 
    return _map_h1[name]; 
  else 
    return 0;
} 


TH2* HistBooker::get_h2(std::string name)
{ 
  if(_map_h2.find(name)!=_map_h2.end()) 
    return _map_h2[name]; 
  else 
    return 0;
} 


TH3* HistBooker::get_h3(std::string name)
{ 
  if(_map_h3.find(name)!=_map_h3.end()) 
    return _map_h3[name]; 
  else 
    return 0;
} 


TH1* HistBooker::book_profile1(std::string name,int nbinx,
			       double xmin,double xmax,
			       double ymin,double ymax)
{
  
  std::vector<std::string> name_title;
  name_title = split(name,':');
  std::string sname = name_title[0]; 
  std::string title = name_title[1]; 
  
  if(_map_h1.find(sname)!=_map_h1.end()){
    return _map_h1[sname]; 
  }else{
    TProfile* h;
    if(ymin<ymax) h = new TProfile(sname.c_str(),title.c_str(),nbinx,xmin,xmax,ymin,ymax);
    else  h=  new TProfile(sname.c_str(),title.c_str(),nbinx,xmin,xmax);
    h->SetDirectory(0);
    h->Sumw2();
    std::pair<std::string, TH1* > pr(sname,h);
    _map_h1.insert(pr);
    std::cout << sname.c_str() <<" [ booked ... ]" <<std::endl;
    return h;
  }
  
}

TH2* HistBooker::book_profile2(std::string name,int nbinx,double xmin,double xmax,int nbiny, double ymin,double ymax, double zmin, double zmax){
  std::vector<std::string> name_title;
  name_title = split(name,':');
  std::string sname = name_title[0]; 
  std::string title = name_title[1]; 
  
  if(_map_h2.find(sname)!=_map_h2.end()){
    return _map_h2[sname]; 
  }else{
    TProfile2D* h;
    // title
    if(zmin<zmax) h = new TProfile2D(sname.c_str(),title.c_str(),nbinx,xmin,xmax,nbiny,ymin,ymax,zmin,zmax);
    else  h=  new TProfile2D(sname.c_str(),title.c_str(),nbinx,xmin,xmax,nbiny,ymin,ymax);
    h->SetDirectory(0);
    h->Sumw2();
    std::pair<std::string, TH2* > pr(sname,h);
    _map_h2.insert(pr);
    std::cout << sname.c_str() <<" [ booked ... ]" <<std::endl;
    return h;
  }
}



void HistBooker::write_histograms(std::string file)
{
  TFile *f = new TFile(file.c_str(),"RECREATE","histograms");
  // save the 1d histograms
  for (std::map<std::string,TH1*>::iterator i_h= _map_h1.begin();i_h!=_map_h1.end();i_h++)
    {
      std::cout<<" writing --> " <<i_h->first<<std::endl;
      f->cd();
      std::vector<std::string> hist_dir;
      hist_dir = split(i_h->first,'/');
      std::string dirname(""),lastname("");
      
      if(hist_dir.size()>0){
	for(unsigned int i=1;i< hist_dir.size()-1; i++){
	  dirname += "/"+hist_dir[i];
	  f->cd();
	  TDirectory *det = f->GetDirectory(dirname.c_str());
	  if(det == 0){
	    f->cd(lastname.c_str());
	    gDirectory->cd();
	    TDirectory* detn = gDirectory->mkdir(hist_dir[i].c_str(),hist_dir[i].c_str());
	    if (detn!=0) detn->cd();
	    det=f->GetDirectory(dirname.c_str());
	  }
	  if(det!=0) det->cd();
	  lastname = dirname;
	}
	i_h->second->SetName(hist_dir[hist_dir.size()-1].c_str());
      }
      i_h->second->Write();
      delete i_h->second;
    }
  // save the 2d histogram
  for (std::map<std::string,TH2*>::iterator i_h= _map_h2.begin();i_h!=_map_h2.end();i_h++)
    {
      std::cout<<" writing --> " <<i_h->first<<std::endl;
      f->cd();
      std::vector<std::string> hist_dir;
      hist_dir = split(i_h->first,'/');
      std::string dirname(""),lastname("");
      if(hist_dir.size()>0){
	for(unsigned int i=1;i< hist_dir.size()-1; i++){
	  dirname += "/"+hist_dir[i];
	  f->cd();
	  TDirectory *det = f->GetDirectory(dirname.c_str());
	  if(det == 0){
	    f->cd(lastname.c_str());
	    gDirectory->cd();
	    TDirectory* detn = gDirectory->mkdir(hist_dir[i].c_str(),hist_dir[i].c_str());
	    if (detn!=0) detn->cd();
	    det=f->GetDirectory(dirname.c_str());
	  }
	  if(det!=0) det->cd();
	  lastname = dirname;
	}
	i_h->second->SetName(hist_dir[hist_dir.size()-1].c_str());
      }
      i_h->second->Write();
      delete i_h->second;
    }

  // save the 3d histogram
  for (std::map<std::string,TH3*>::iterator i_h= _map_h3.begin();i_h!=_map_h3.end();i_h++)
    {
      std::cout<<" writing --> " <<i_h->first<<std::endl;
      f->cd();
      std::vector<std::string> hist_dir;
      hist_dir = split(i_h->first,'/');
      std::string dirname(""),lastname("");
      if(hist_dir.size()>0){
	for(unsigned int i=1;i< hist_dir.size()-1; i++){
	  dirname += "/"+hist_dir[i];
	  f->cd();
	  TDirectory *det = f->GetDirectory(dirname.c_str());
	  if(det == 0){
	    f->cd(lastname.c_str());
	    gDirectory->cd();
	    TDirectory* detn = gDirectory->mkdir(hist_dir[i].c_str(),hist_dir[i].c_str());
	    if (detn!=0) detn->cd();
	    det=f->GetDirectory(dirname.c_str());
	  }
	  if(det!=0) det->cd();
	  lastname = dirname;
	}
	i_h->second->SetName(hist_dir[hist_dir.size()-1].c_str());
      }
      i_h->second->Write();
      delete i_h->second;
    }
  
  f->Write();
  f->Close(); 
}


std::vector<std::string> HistBooker::split(std::string string_list, char sep){
  std::vector<std::string> v_string;
  std::istringstream ss( string_list );
  while (!ss.eof())
    {
      std::string x;               
      getline( ss, x, sep );  
      v_string.push_back(x);
    }
  return v_string;
}
