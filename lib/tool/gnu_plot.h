#pragma once

#include <fmt/format.h>
#include <optional>
#include <vector>
#include <algorithm>
#include <gnuplot-iostream.h>

namespace MusikBox::Tools {

class Plotter {
public:
    Plotter() {}

    template <typename T1, typename T2>
    void Plot1D(const std::vector<std::pair<T1, T2>>& data) {
        if (!x_max.has_value()) {
            x_max = data.size();
        }

        if (!y_min.has_value()) {
            auto minIt = std::min_element(data.begin(), data.end(),
                                          [](const std::pair<T1, T2>& a, const std::pair<T1, T2>& b) {
                                          return a.second < b.second;
                                          });
            if (minIt != data.end()) { y_min = static_cast<float>(minIt->second); }
        }

        if (!y_max.has_value()) {
            auto maxIt = std::max_element(data.begin(), data.end(),
                                          [](const std::pair<T1, T2>& a, const std::pair<T1, T2>& b) {
                                          return a.second < b.second;
                                          });
            if (maxIt != data.end()) { y_max = static_cast<float>(maxIt->second); }
        }

        std::string command = fmt::format("set xrange [{}:{}]\nset yrange [{:.2f}:{:.2f}]\n",
                                          x_min,
                                          x_max.value_or(100),
                                          y_min.value_or(0),
                                          y_max.value_or(100));
        gp << command;
        gp << "plot '-'\n";
        gp.send1d(data);
    }

    void Plot1D(const std::vector<std::pair<float, std::complex<double>>>& data) {
        std::vector<std::pair<float, double>> data_i;
        data_i.resize(data.size());
        for (size_t i = 0; i < data.size(); i++) {
            float x = data[i].first;
            double y = static_cast<double>(data[i].second.real());
            data_i[i] = std::pair(x, y);
        }
        Plot1D<float, double>(data_i);
    }

    void SetBounds(float x_i,
                   std::optional<float> x_n,
                   std::optional<float> y_i,
                   std::optional<float> y_n) {
        x_min = x_i;
        x_max = x_n;
        y_min = y_i;
        y_max = y_n;
    }

private:
    Gnuplot gp;
    float x_min = 0.0;
    std::optional<float> x_max;
    std::optional<float> y_min;
    std::optional<float> y_max;
};

} // namespace MusikBox::Tools
