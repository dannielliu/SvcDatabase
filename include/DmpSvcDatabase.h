#ifndef DmpSvcDatabase_H
#define DmpSvcDatabase_H

#include "DmpVSvc.h"
//#include <boost/filesystem.hpp>     // path 
#include <fstream>
#include <vector>
//using std::map;
//using std::string;
using namespace std;
struct valueset{
  vector<double> value;
	//string type;
};

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
  //string fPedBase;
	//string fMipsBase;
	string fPackage;
	string fSubDetector;
	string fConnectorPathBgo;
	string fConnectorPathPsd;
	string fConnectorPathStk;
	string fConnectorPathNud;
  map<string, std::ios_base::openmode> fModeMap;
	//std::ios_base::openmode fOpenMode;  //possible mode are app, ate, in, out,binary, trunc
  map<string,string> fReadList;// first string is arguement type, second is filename
  map<string,string> fWriteList;
	map<string,fstream*> outfiles;
	//fstream fPedFile;
	//fstream fMipsFile;
	map<string,map<int,valueset> > ParMap;

public:
  template<typename type1>
  bool AppendData(int position, type1 value,string pkgType=""){
	  if(pkgType==""){
		  pkgType=outfiles.begin()->first;
		}
    if(outfiles.size()>0){
		  *(outfiles[pkgType])<<position<<"\t"<<value<<std::endl;
		}
		else{
		  DmpLogError<<"nothing to save, you should not use this function.:-("<<DmpLogEndl;
		}
		/*
		if (fCurrentWrite == "Ped"||pkgType=="Ped"){ 
	    fPedFile<<position<<"\t"<<value<<std::endl;
	  }
	  else if (fCurrentWrite == "Mips"||pkgType=="Mips"){
	    fMipsFile<<position<<"\t"<<value<<std::endl;
	  } 
	  else{
	    DmpLogError<<"I don't know where to save the data."<<DmpLogEndl;
		  return false;
	  }*/ 
    return true;
  } 
  template<typename type1>
  bool AppendData(int position, type1 value1, type1 value2,string pkgType=""){
    if(pkgType==""){
		  pkgType=outfiles.begin()->first;
		}
    if(outfiles.size()>0){
		  *(outfiles[pkgType])<<position<<"\t"<<value1<<"\t"<<value2<<std::endl;
		}
		else{
		  DmpLogError<<"nothing to save, you should not use this function.:-("<<DmpLogEndl;
		}

		/*
		if (fCurrentWrite == "Ped"||pkgType=="Ped"){ 
	    fPedFile<<position<<"\t"<<value1<<"\t"<<value2<<std::endl;
	  }
	  else if (fCurrentWrite == "Mips"||pkgType=="Mips"){
	    fMipsFile<<position<<"\t"<<value1<<"\t"<<value2<<std::endl;
	  } 
	  else{ 
	    DmpLogError<<"I don't know where to save the data."<<DmpLogEndl;
		  return false;
	  }*/ 
    return true;
  }

	double GetData(int position,int id=0,string pkgType=""){
    double result=-1;
		if(pkgType==""){
		  pkgType=ParMap.begin()->first;;
		}
		if(ParMap[pkgType].size()>0){
		  return ParMap[pkgType][position].value.at(id);
		}
		else{
		  DmpLogError<<ParMap.begin()->first<<" is empty."<<DmpLogEndl;
		}

	  return result;
  } 

  bool LoadPar(fstream &file, string pkgType="");
	void ShowPar(string pkgType="");

};

extern DmpSvcDatabase *gDatabaseSvc;

#endif
