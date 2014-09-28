#include <boost/algorithm/string.hpp>
#include "DmpSvcDatabase.h"
#include <stdlib.h>
#include <vector>

//-------------------------------------------------------------------
DmpSvcDatabase::DmpSvcDatabase()
 :DmpVSvc("Database"),
 //fPedBase(""),  //set pedestal file name
 //fMipsBase(""), //set mips file name
 fPackage("Calibration"),
 fSubDetector("BGO"),
 //fOpenMode(std::ios::in),
 fDatabasePath((string)getenv("DMPSWWORK")+"/Database")
 //fPedFile(0),
 //fMipsFile(0)
{
  fReadList.clear();
	fWriteList.clear();
  ParMap.clear();
	outfiles.clear();
  //PedMap.clear();
	//MipsMap.clear();
  //fDatabasePath += "/Database";
  OptMap.insert(make_pair("Pedestal/Write",  0));
	OptMap.insert(make_pair("Pedestal/Read",   1));
	OptMap.insert(make_pair("Pedestal/Update", 2));
	OptMap.insert(make_pair("Mips/Write",      3));
	OptMap.insert(make_pair("Mips/Read",       4));
	OptMap.insert(make_pair("Mips/Update",     5));
	OptMap.insert(make_pair("Detector/STK",    6));
	OptMap.insert(make_pair("Detector/BGO",    7));
	OptMap.insert(make_pair("Detectpr/PSD",    8));
	OptMap.insert(make_pair("Detector/Nud",    9));
	OptMap.insert(make_pair("Database/Path",  10));
	OptMap.insert(make_pair("Package",        11));
}

//-------------------------------------------------------------------
DmpSvcDatabase::~DmpSvcDatabase(){
} 

//-------------------------------------------------------------------
bool DmpSvcDatabase::Initialize(){
  // deal with pedestal data.
	//std::cout<<"Current work directory:"<< fCurrentRead <<std::endl;
	// read input data
	if(fReadList.size()>0){
	  for(map<string,string>::iterator it=fReadList.begin();it!=fReadList.end();it++){
		  fstream datain;
			string fullname;
			fullname = fDatabasePath+'/'+fPackage +'/'+fSubDetector+'/' + it->second;
			datain.open(fullname,ios::in);
			if(datain){
	      DmpLogInfo<<" Open "<<fullname<<" Successful."<<DmpLogEndl;
			}
			else{
	      DmpLogInfo<<" Open "<<fullname<<" Failed."<<DmpLogEndl;
			  continue;
			}
			LoadPar(datain,it->first);
			datain.close();
	    DmpLogInfo<<fullname<<" Closed."<<DmpLogEndl;
		}
	}
	// create output list
	if(fWriteList.size()>0){
	  for(map<string,string>::iterator it=fWriteList.begin();it!=fWriteList.end();it++){
		  fstream *dataout;
			dataout=new fstream();
			string fullname;
			fullname = fDatabasePath+'/'+fPackage +'/'+fSubDetector+'/' + it->second;
			dataout->open(fullname,fModeMap[it->first]);
			outfiles.insert(make_pair(it->first,dataout));
			if(dataout){
	      DmpLogInfo<<" Open "<<fullname<<" Successful."<<DmpLogEndl;
			}
			else{
	      DmpLogInfo<<" Open "<<fullname<<" Failed."<<DmpLogEndl;
			  continue;
			}
		}
	}
	/*
  if(fPedBase!=""){
    fPedBase = fDatabasePath+'/'+fPackage +'/'+fSubDetector+'/' + fPedBase;
	  fPedFile.open(fPedBase,fModeMap["Ped"]);
	  if (fPedFile){
	    DmpLogInfo<<" Open "<<fPedBase<<" Successful."<<DmpLogEndl;
		  if (fModeMap["Ped"]==ios::in){
			  LoadPar(fPedFile,"Ped");
				fPedFile.close();
				DmpLogInfo<<fPedBase<<" Closed"<<DmpLogEndl;
		  }
	  }
	  else{  
	    DmpLogError<<" Can not open pedestal file. "<<fPedBase<<DmpLogEndl;
	  }	
	}
  
	// deal with mips file
	if(fMipsBase!=""){
	  fMipsBase = fDatabasePath+'/'+fPackage +'/'+fSubDetector+'/' + fMipsBase;
  	fMipsFile.open(fMipsBase,fModeMap["Mips"]);
	  if (fMipsFile){
	    DmpLogInfo<<" Open "<<fMipsBase<<" Successful."<<DmpLogEndl;
  	  if (fModeMap["Mips"]==ios::in){
			  LoadPar(fMipsFile,"Mips");
				fMipsFile.close();
				DmpLogInfo<<fMipsBase<<" Closed"<<DmpLogEndl;
		  }
	  }
	  else{ 
	    DmpLogError<<" Can not open Mips file. "<<fMipsBase<<DmpLogEndl;
	  } 
   }
*/
  return true;
} 

//-------------------------------------------------------------------
bool DmpSvcDatabase::Finalize(){
  //std::cout<<"fPed   "<<fPedFile.tellp()<<std::endl;
 	if(outfiles.size()>0){
	  for(map<string,fstream*>::iterator it=outfiles.begin();it!=outfiles.end();it++){
		  it->second->close();
      DmpLogInfo<<it->first<<" file has been Closed."<<DmpLogEndl;
		}
	}
/*
  if (fPedFile){
	  fPedFile.close();
		DmpLogInfo<<fPedBase<<" Closed."<<DmpLogEndl;
	} 
	if (fMipsFile){
	  fMipsFile.close();
		DmpLogInfo<<fMipsBase<<" Closed."<<DmpLogEndl;
	 }
*/
  return true;
} 

void  DmpSvcDatabase::Set(const string &type, const string &value){
  if(OptMap.find(type) == OptMap.end()){
    DmpLogError<<"No argument type: "<<type<<DmpLogEndl;
		DmpLogInfo<<"Possible option are "<<DmpLogEndl;
		for (map<string,short>::iterator anOpt= OptMap.begin();anOpt!=OptMap.end();anOpt++){
		  DmpLogInfo<<anOpt->first<<DmpLogEndl;
		} 
		//throw;
	}

	switch (OptMap[type])
  {
	  case 0:
		  //fPedBase = value;
			fModeMap.insert(make_pair("Ped",ios::out));
			fWriteList.insert(make_pair("Ped",value));
		  break;
		case 1:
		  //fPedBase = value;
			fModeMap.insert(make_pair("Ped",ios::in));
			fReadList.insert(make_pair("Ped",value));
		  break;
		case 2:
		  //fPedBase = value;
			fModeMap.insert(make_pair("Ped",ios::app));
			fWriteList.insert(make_pair("Ped",value));
		  break;
		case 3:
		  //fMipsBase = value;
			fModeMap.insert(make_pair("Mips",ios::out));
			fWriteList.insert(make_pair("Mips",value));
		  break;
		case 4:
		  //fMipsBase = value;
			fModeMap.insert(make_pair("Mips",ios::in));
			fReadList.insert(make_pair("Mips",value));
		  break;
		case 5:
		  //fMipsBase = value;
			fModeMap.insert(make_pair("Mips",ios::app));
			fWriteList.insert(make_pair("Mips",value));
		  break;
	  case 6:
		  fSubDetector = "STK";
		  break;
		case 7:
		  fSubDetector = "BGO";
		  break;
		case 8:
		  fSubDetector = "PSD";
		  break;
		case 9:
		  fSubDetector = "NUD";
		  break;
	  case 10:
		  fDatabasePath = value;
			break;
	  case 11:
		  fPackage = value;
			break;
	}   
}

bool DmpSvcDatabase::LoadPar(fstream &file,string pkgType){
  if(pkgType==""){
	  pkgType=fReadList.begin()->first;
	}
  string tmpstr;
  vector<string> strings;
  int tmpPos;
  valueset tmpValue;
	map<int,valueset> tmpset;
	DmpLogInfo<<"Reading ..."<<DmpLogEndl;
	getline(file,tmpstr);                                 //read out by line
	while(!file.eof()){
	  boost::split(strings,tmpstr,boost::is_any_of("\t")); //split by \t
	  tmpPos=atoi(strings.at(0).c_str());                  //convert string to int, first is position
	  tmpValue.value.push_back(atof(strings.at(1).c_str()));
		if (strings.size()>1)
		  tmpValue.value.push_back(atof(strings.at(2).c_str()));
    tmpset.insert(make_pair(tmpPos,tmpValue));
		tmpValue.value.clear();
    getline(file,tmpstr);                               //read out by line
  }
	ParMap.insert(make_pair(pkgType,tmpset));

	return true;
}

void DmpSvcDatabase::ShowPar(string pkgType){
  if(pkgType==""){
	  pkgType=ParMap.begin()->first;;
	}
	  std::cout<<"parameter table:\n"<<std::endl;
  for(std::map<int,valueset>::iterator it=ParMap[pkgType].begin();it!=ParMap[pkgType].end();it++){
	  std::cout<<it->first<<std::endl;
		for(int i=0;i<it->second.value.size();i++){
		  std::cout<<'\t'<<it->second.value.at(i)<<std::endl;
		}
	}
}

DmpSvcDatabase *gDatabaseSvc = DmpSvcDatabase::GetInstance();
