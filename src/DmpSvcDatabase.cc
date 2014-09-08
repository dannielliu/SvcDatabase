#include <boost/algorithm/string.hpp>
#include "DmpSvcDatabase.h"
#include <stdlib.h>
#include <vector>

//-------------------------------------------------------------------
DmpSvcDatabase::DmpSvcDatabase()
 :DmpVSvc("Database"),
 fPedBase("Ped"),
 fMipsBase("Mips"),
 fPackage("Calibration"),
 fSubDetector("BGO"),
 //fOpenMode(std::ios::in),
 fDatabasePath((string)getenv("DMPSWWORK")+"/Database"),
 fPedFile(0),
 fMipsFile(0)
{
  PedMap.clear();
	MipsMap.clear();
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
	std::cout<<"Current work directory:"<< fCurrentWork <<std::endl;
  if(fCurrentWork=="Ped"){
    fPedBase = fDatabasePath+'/'+fPackage +'/'+fSubDetector+'/' + fPedBase;
	  fPedFile.open(fPedBase,fModeMap["Ped"]);
	  string tmpstr;
	  vector<string> strings;
	  int tmpPos;
	  double tmpValue;
	  if (fPedFile){
	    DmpLogInfo<<" Open "<<fPedBase<<" Successful."<<DmpLogEndl;
		  if (fModeMap["Ped"]==ios::in){
				DmpLogInfo<<"Reading ..."<<DmpLogEndl;
			  getline(fPedFile,tmpstr);                                 //read out by line
		    while(!fPedFile.eof()){
				  //DmpLogInfo<<"Reading ..."<<DmpLogEndl;
			    //getline(fPedFile,tmpstr);                                 //read out by line
				  boost::split(strings,tmpstr,boost::is_any_of("\t"));      //split by \t
		      //fPedFile>>tmpPos>>tmpValue;
				  tmpPos=atoi(strings.at(0).c_str());                       //convert string to int, first is position
				  tmpValue=atof(strings.at(1).c_str());
					std::cout<<tmpPos<<' '<<tmpValue<<std::endl;
          PedMap.insert(make_pair(tmpPos,tmpValue));
			    getline(fPedFile,tmpstr);                                 //read out by line
			  }
		  }
	  }
	  else{  
	    DmpLogError<<" Can not open pedestal file. "<<fPedBase<<DmpLogEndl;
	  }	
	}
  
	// deal with mips file
	if(fCurrentWork=="Mips"){
	  fMipsBase = fDatabasePath+'/'+fPackage +'/'+fSubDetector+'/' + fMipsBase;
  	fMipsFile.open(fMipsBase,fModeMap["Mips"]);
 	  string tmpstr;
	  vector<string> strings;
	  int tmpPos;
	  double tmpValue;
	  if (fMipsFile){
	    DmpLogInfo<<" Open "<<fMipsBase<<" Successful."<<DmpLogEndl;
  	  if (fModeMap["Mips"]==ios::in){
				DmpLogInfo<<"Reading ..."<<DmpLogEndl;
			  getline(fMipsFile,tmpstr);                                 //read out by line
		    while(!fMipsFile.eof()){
				  //DmpLogInfo<<"Reading ..."<<DmpLogEndl;
			    //getline(fPedFile,tmpstr);                                 //read out by line
				  boost::split(strings,tmpstr,boost::is_any_of("\t"));      //split by \t
		      //fPedFile>>tmpPos>>tmpValue;
				  tmpPos=atoi(strings.at(0).c_str());                       //convert string to int, first is position
				  tmpValue=atof(strings.at(1).c_str());
					std::cout<<tmpPos<<' '<<tmpValue<<std::endl;
          MipsMap.insert(make_pair(tmpPos,tmpValue));
			    getline(fMipsFile,tmpstr);                                 //read out by line
			  }
		  }
	  }
	  else{ 
	    DmpLogError<<" Can not open Mips file. "<<fMipsBase<<DmpLogEndl;
	  } 
   }

  return true;
} 

//-------------------------------------------------------------------
//bool DmpSvcDatabase::ProcessThisEvent(){
//  return true;
//}

//-------------------------------------------------------------------
bool DmpSvcDatabase::Finalize(){
  std::cout<<"fPed   "<<fPedFile.tellp()<<std::endl;
  if (fPedFile){
	  fPedFile.close();
		DmpLogInfo<<fPedBase<<" Closed."<<DmpLogEndl;
	} 
	if (fMipsFile){
	  fMipsFile.close();
		DmpLogInfo<<fMipsBase<<" Closed."<<DmpLogEndl;
	 }

  return true;
} 

void  DmpSvcDatabase::Set(const string &type, const string &value){
  if(OptMap.find(type) == OptMap.end()){
    DmpLogError<<"No argument type: "<<type<<DmpLogEndl;
		DmpLogInfo<<"Possible option are "<<DmpLogEndl;
		for (map<string,short>::iterator anOpt= OptMap.begin();anOpt!=OptMap.end();anOpt++){
		  DmpLogInfo<<anOpt->first<<DmpLogEndl;
		} 
		throw;
	}
	
	switch (OptMap[type])
  {
	  case 0:
		  fPedBase = value;
			fModeMap.insert(make_pair("Ped",ios::out));
			fCurrentWork = "Ped";
		  break;
		case 1:
		  fPedBase = value;
			fModeMap.insert(make_pair("Ped",ios::in));
			fCurrentWork = "Ped";
		  break;
		case 2:
		  fPedBase = value;
			fModeMap.insert(make_pair("Ped",ios::app));
			fCurrentWork = "Ped";
		  break;
		case 3:
		  fMipsBase = value;
			fModeMap.insert(make_pair("Mips",ios::out));
			fCurrentWork = "Mips";
		  break;
		case 4:
		  fMipsBase = value;
			fModeMap.insert(make_pair("Mips",ios::in));
			fCurrentWork = "Mips";
		  break;
		case 5:
		  fMipsBase = value;
			fModeMap.insert(make_pair("Mips",ios::app));
			fCurrentWork = "Mips";
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
/*
template<typename type1>
bool DmpSvcDatabase::AppendData(int position, type1 value){
  if (fCurrentWork == "Ped"){
	  fPedFile<<position<<"\t"<<value<<std::endl;
	}
	else if (fCurrentWork == "Mips"){
	  fMipsFile<<position<<"\t"<<value<<std::endl;
	}
	else{
	  DmpLogError<<"I don't know where to save the data."<<DmpLogEndl;
		return false;
	}
  return true;
}
*/
/*
template<typename v_type>
v_type GetData(int position){
  v_type result;

	return result;
}
*/
DmpSvcDatabase *gDatabaseSvc = DmpSvcDatabase::GetInstance();
