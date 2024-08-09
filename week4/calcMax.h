#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <stdexcept>
#include <limits>

// Function to find the max value in column 1 and its index
std::tuple<double, size_t> getMaxValue(const std::vector<std::vector<std::string>>& dataset) {
    if (dataset.empty() || dataset[0].size() < 2) {
        throw std::invalid_argument("Dataset must have at least two columns.");
    }

    double max_value = std::numeric_limits<double>::lowest();
    size_t max_index = 0;
    bool found = false;

    for (size_t i = 0; i < dataset.size(); i++) {
        if (dataset[i].size() < 2) {
            continue; // skip rows with less than two columns
        }

        try {
            double value = std::stod(dataset[i][1]);
            if (value > max_value) {
                max_value = value;
                max_index = i;
                found = true;
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Warning: Skipping invalid number at row " << i << ": " << dataset[i][1] << std::endl;
        }
    }

    if (!found) {
        throw std::runtime_error("No valid numbers found in column 1.");
    }

    return std::make_tuple(max_value, max_index);
}
