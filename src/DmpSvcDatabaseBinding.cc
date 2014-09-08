#include <boost/python.hpp>
#include "DmpSvcDatabase.h"

BOOST_PYTHON_MODULE(libDmpSvcDatabase){
  using namespace boost::python;

  //class_<DmpSvcDatabase,boost::noncopyable,bases<DmpVSvc> >("DmpSvcDatabase",init<>());
  class_<DmpSvcDatabase,boost::noncopyable,bases<DmpVSvc> >("DmpSvcDatabase",init<>())
    //.def("GetInstance", &DmpSvcDatabase::GetInstance)
    .def("GetInstance", &DmpSvcDatabase::GetInstance,return_value_policy<reference_existing_object>())
    .staticmethod("GetInstance")
  ;
}
