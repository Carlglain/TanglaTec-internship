//Looking for max using gaussian fit in c++
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <stdexcept>
#include <limits>
#include <cmath>

double gaussian_pdf(double x, double mu, double sigma, double amplitude) {
    return amplitude * exp(-(x - mu) * (x - mu) / (2.0 * sigma * sigma));
}


// Function to calculate mean and standard deviation
std::tuple<double, double> calculateMeanAndSigma(const std::vector<double>& values) {
    double sum = 0.0;
    for (double value : values) {
        sum += value;
    }
    double mean = sum / values.size();

    double variance_sum = 0.0;
    for (double value : values) {
        variance_sum += (value - mean) * (value - mean);
    }
    double variance = variance_sum / (values.size() - 1); // Corrected division
    double sigma = sqrt(variance);

    return std::make_tuple(mean, sigma);
}

// Function to extract numeric values from dataset
std::vector<double> extractValues(const std::vector<std::vector<std::string>>& dataset, size_t column) {
    std::vector<double> values;
    for (size_t i = 0; i < dataset.size(); i++) {
        if (dataset[i].size() <= column) {
            std::cerr << "Warning: Skipping row " << i << " due to insufficient columns." << std::endl;
            continue; // skip rows with less than the required columns
        }

        try {
            double value = std::stod(dataset[i][column]);
            values.push_back(value);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Warning: Skipping invalid number at row " << i << ": " << dataset[i][column] << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Warning: Number out of range at row " << i << ": " << dataset[i][column] << std::endl;
        }
    }

    if (values.empty()) {
        throw std::runtime_error("No valid numbers found in the specified column.");
    }

    return values;
}


// Function to calculate Gaussian peak value and the value of x where it occurs
std::tuple<double, double> getGaussianPeak(double mu, double sigma, double amplitude) {
    return std::make_tuple(amplitude, mu);
}