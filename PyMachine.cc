#include <math.h>

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "stack_machine.h"

bool CheckPrime(long long n){
  if(n <= 0) return false;
  if(n == 1) return true; //1 is OK
  if(n == 2) return true;
  if(!(n & 1)) return false;

  long long sqrtx = sqrt(n);
  for(int k = 3; k <= sqrtx; k += 2){
    if(n % k == 0) return false;
  }

  return true;
}

using namespace boost::python;
BOOST_PYTHON_MODULE(PyMachine)
{
  class_<StackMachine>("PyMachine", init<int, long long, int, int, int>())
    .def("execute", &StackMachine::Execute)
    .def("load", &StackMachine::Load)
    .def("show", &StackMachine::ShowCode)
    .def("getOutput", &StackMachine::GetOutput)
    .add_property("maxbits", &StackMachine::get_max_bits, &StackMachine::set_max_bits);
    ;

  class_< std::vector<long long> >("VectorOfLongLong")
    .def(vector_indexing_suite< std::vector<long long> >() )
    ;

  def("checkPrime", CheckPrime);
}
