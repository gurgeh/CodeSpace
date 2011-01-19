#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "stack_machine.h"

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
}
