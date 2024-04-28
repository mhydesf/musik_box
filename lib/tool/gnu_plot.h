#pragma once

#include <fmt/format.h>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <gnuplot-iostream.h>

namespace MusikBox::Tools {

class Plotter {
public:
    Plotter() {}

    template <typename T1, typename T2>
    void Plot1D(const std::vector<std::pair<T1, T2>>& data) {
        auto maxIt = std::max_element(data.begin(), data.end(),
            [](const std::pair<T1, T2>& a, const std::pair<T1, T2>& b) {
                return a.second < b.second;
            });

        auto minIt = std::min_element(data.begin(), data.end(),
            [](const std::pair<T1, T2>& a, const std::pair<T1, T2>& b) {
                return a.second < b.second;
            });

        if (maxIt != data.end() && minIt != data.end()) {
            std::string command = fmt::format("set xrange [0:{}]\nset yrange [{:.2f}:{:.2f}]\n",
                                              data.size(),
                                              static_cast<float>(minIt->second),
                                              static_cast<float>(maxIt->second));
            gp << command;
            gp << "plot '-'\n";
            gp.send1d(data);
        }
    }

    void Plot1D(const std::vector<std::pair<float, std::complex<double>>>& data) {
        std::vector<std::pair<float, int16_t>> data_i;
        data_i.resize(data.size());
        for (size_t i = 0; i < data.size(); i++) {
            float x = data[i].first;
            int16_t y = static_cast<int16_t>(data[i].second.real());
            data_i[i] = std::pair(x, y);
        }
        Plot1D<float, int16_t>(data_i);
    }

private:
    Gnuplot gp;
};

} // namespace MusikBox::Tools
