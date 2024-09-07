#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <algorithm>

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

// Gaussian PDF
double gaussian_pdf(double x, double y, double mu_x, double mu_y, double sigma_x, double sigma_y, double amplitude) {
    return amplitude * exp(-((x - mu_x) * (x - mu_x) / (2.0 * sigma_x * sigma_x) + (y - mu_y) * (y - mu_y) / (2.0 * sigma_y * sigma_y)));
}
// Function to calculate the sum of squared errors
// Function to calculate the sum of squared errors for 2D
double calculateSSE(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z, double mu_x, double mu_y, double sigma_x, double sigma_y, double amplitude) {
    double sse = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        double predicted = gaussian_pdf(x[i], y[i], mu_x, mu_y, sigma_x, sigma_y, amplitude);
        sse += (z[i] - predicted) * (z[i] - predicted); // Sum of squared errors
    }
    return sse;
}

// Function to fit Gaussian parameters using gradient descent in 2D
std::tuple<double, double, double, double, double> fitGaussian(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z) {
    double mu_x = x[std::distance(z.begin(), std::max_element(z.begin(), z.end()))];
    double mu_y = y[std::distance(z.begin(), std::max_element(z.begin(), z.end()))];
    double sigma_x = 1.0; // Initial guess for sigma_x
    double sigma_y = 1.0; // Initial guess for sigma_y
    double amplitude = *std::max_element(z.begin(), z.end()); // Initial amplitude

    double learning_rate = 0.001;
    for (int iter = 0; iter < 10000; ++iter) {
        double sse = calculateSSE(x, y, z, mu_x, mu_y, sigma_x, sigma_y, amplitude);

        double dmu_x = 0.0, dmu_y = 0.0, dsigma_x = 0.0, dsigma_y = 0.0, damplitude = 0.0;

        for (size_t i = 0; i < x.size(); ++i) {
            double predicted = gaussian_pdf(x[i], y[i], mu_x, mu_y, sigma_x, sigma_y, amplitude);
            double error = z[i] - predicted;

            dmu_x += error * (-1.0 / (sigma_x * sigma_x)) * (x[i] - mu_x) * predicted;
            dmu_y += error * (-1.0 / (sigma_y * sigma_y)) * (y[i] - mu_y) * predicted;
            dsigma_x += error * (1.0 / (sigma_x * sigma_x * sigma_x)) * (x[i] - mu_x) * predicted;
            dsigma_y += error * (1.0 / (sigma_y * sigma_y * sigma_y)) * (y[i] - mu_y) * predicted;
            damplitude += error * predicted;
        }

        mu_x += learning_rate * dmu_x;
        mu_y += learning_rate * dmu_y;
        sigma_x += learning_rate * dsigma_x;
        sigma_y += learning_rate * dsigma_y;
        amplitude += learning_rate * damplitude;

        // Optional: Check for convergence
        if (std::abs(dmu_x) < 1e-6 && std::abs(dmu_y) < 1e-6 && 
            std::abs(dsigma_x) < 1e-6 && std::abs(dsigma_y) < 1e-6 && 
            std::abs(damplitude) < 1e-6) {
            break;
        }
    }

    return std::make_tuple(mu_x, mu_y, sigma_x, sigma_y, amplitude);
}

// Function to extract numeric values from dataset and fit Gaussian
std::tuple<double, double, double> findGaussianPeak(const std::vector<std::vector<std::string>>& dataset, size_t x_column, size_t y_column, size_t z_column) {
    std::vector<double> x = extractValues(dataset, x_column);
    std::vector<double> y = extractValues(dataset, y_column);
    std::vector<double> z = extractValues(dataset, z_column);

    auto [mu_x, mu_y, sigma_x, sigma_y, amplitude] = fitGaussian(x, y, z);
    return std::make_tuple(mu_x, mu_y, amplitude); // Return the x and y values where the max occurs, and the max value
}