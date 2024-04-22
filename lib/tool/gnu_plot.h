#pragma once

#include <vector>
#include <fmt/format.h>
#include <algorithm>
#include "gnuplot-iostream.h"

namespace MusikBox::Tools {

class Plotter {
public:
    Plotter() {}

    template <typename T>
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

    void Plot1D(const std::vector<std::complex<double>>& data) {
        std::vector<int16_t> data_i;
        data_i.resize(data.size());
        for (size_t i = 0; i < data.size(); i ++) {
            data_i[i] = static_cast<int16_t>(data[i].real());
        }
        Plot1D<int16_t>(data_i);
    }

private:
    Gnuplot gp;
};

} // namespace MusikBox::Tools
