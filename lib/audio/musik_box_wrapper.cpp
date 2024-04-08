#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <vector>

int simple_func(int a, int b) { return a + b; }

std::vector<int> get_vec() {
    return {1,2,3};
}

PYBIND11_MODULE(musik_box, m) {
    m.def("simple_func", &simple_func);
    m.def("get_vec", &get_vec);
}
