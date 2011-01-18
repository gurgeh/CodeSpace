#include <boost/python.hpp>

#include "stack_machine.h"

using namespace boost::python;
BOOST_PYTHON_MODULE(PyMachine)
{
  class_<StackMachine>("PyMachine", init<int, long long, int, int>())
    .def("execute", &StackMachine::Execute)
    .def("load", &StackMachine::Load)
    .def("show", &StackMachine::ShowCode)
    .add_property("maxbits", &StackMachine::get_max_bits, &StackMachine::set_max_bits);
    ;
}
