#pragma once

#include <vector>
#include <fmt/format.h>
#include "gnuplot-iostream.h"

namespace MusikBox::Tools {

template <typename T>
class Plotter {
public:
    Plotter() {}

    void Plot1D(const std::vector<T>& data) {
        gp << fmt::format(
            "set xrange [0:{}]\nset yrange [{}:{}]\n",
            data.size(),
            *std::max_element(data.begin(), data.end()),
            *std::min_element(data.begin(), data.end())
        );
        gp << "plot '-'\n";
        gp.send1d(data);
    }

private:
    Gnuplot gp;
};

} // namespace MusikBox::Tools
