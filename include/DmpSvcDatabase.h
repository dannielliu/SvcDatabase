#ifndef DmpSvcDatabase_H
#define DmpSvcDatabase_H

#include "DmpVSvc.h"
//#include <boost/filesystem.hpp>     // path 
#include <fstream>
//using std::map;
//using std::string;
using namespace std;

class DmpSvcDatabase : public DmpVSvc{
/*
 *  DmpSvcDatabase
 *
 */
public:
  DmpSvcDatabase();
  ~DmpSvcDatabase();
  static DmpSvcDatabase* GetInstance(){
	  static DmpSvcDatabase instance;
		return &instance;
	}

  void Set(const string &type,const string &value);
  // if you need to set some options for your algorithm at run time. Overload Set()
  bool Initialize();
  //bool ProcessThisEvent();    // only for algorithm
  bool Finalize();

private:
  string fDatabasePath;
  string fPedBase;
	string fMipsBase;
	string fPackage;
	string fSubDetector;
	string fConnectorPathBgo;
	string fConnectorPathPsd;
	string fConnectorPathStk;
	string fConnectorPathNud;
  map<string, std::ios_base::openmode> fModeMap;
	//std::ios_base::openmode fOpenMode;  //possible mode are app, ate, in, out,binary, trunc
  string fCurrentWork;
	fstream fPedFile;
	fstream fMipsFile;
	map<int,double> PedMap;
	map<int,double> MipsMap;

public:
  template<typename type1>
  bool AppendData(int position, type1 value,string pkgType=""){
    if (fCurrentWork == "Ped"||pkgType=="Ped"){ 
	    fPedFile<<position<<"\t"<<value<<std::endl;
	  }
	  else if (fCurrentWork == "Mips"||pkgType=="Mips"){
	    fMipsFile<<position<<"\t"<<value<<std::endl;
	  } 
	  else{ 
	    DmpLogError<<"I don't know where to save the data."<<DmpLogEndl;
		  return false;
	  } 
    return true;
  } 

	double GetData(int position,string pkgType=""){
    double result=-1;
		if(pkgType=="Ped"||fCurrentWork=="Ped"){
		  return PedMap[position];
		}
		if(pkgType=="Mips"||fCurrentWork=="Mips"){
		  return MipsMap[position];
		}

	  return result;
  } 
  
};

extern DmpSvcDatabase *gDatabaseSvc;

#endif
