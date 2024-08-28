#include "./ETL/ETL.h"
#include <iostream>
#include <string>
#include <Eigen/Dense>
#include <boost/algorithm/string.hpp>
#include <vector>
#include "calcMax.h"
#include "gausianFitMax.h"
#include <utility>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

std::vector<std::vector<std::pair<double, double>>> all_gauss_data; // Store multiple dataset maximum values and their positions
std::vector<std::vector<std::pair<double, double>>> all_direct_data;
void load_data(const std::string& fileLocation) {
    std::ifstream in_file(fileLocation);
    if (!in_file.is_open()) {
        std::cerr << "Error opening input file: " << fileLocation << std::endl;
        return;
    }

    in_file.close();
}

void save_data(const std::string& fileLocation, const std::vector<std::pair<double, double>>& dir) {
    std::ofstream out_file(fileLocation, std::ios::app);
    if (!out_file.is_open()) {
        std::cerr << "Error opening output file: " << fileLocation << std::endl;
        return;
    }
    for (const auto& pair : dir) {
        out_file << pair.first << "," << pair.second << std::endl;
    }
    out_file.close();
}
void appenDataToAllDataSets(double max_value, double peak_pos,std::vector<std::vector<std::pair<double, double>>> library){
   std::vector<std::pair<double, double>> current_data;
    current_data.push_back(std::make_pair(max_value, peak_pos));
    library.push_back(current_data);
}
int main(int argc, char *argv[])
{
    std::string datasetFolder = argv[1];
 
    boost::filesystem::path datasetFolderPath(datasetFolder);

    if (!boost::filesystem::exists(datasetFolderPath) || !boost::filesystem::is_directory(datasetFolderPath)) {
        std::cerr << "Error: Dataset folder '" << datasetFolder << "' does not exist or is not a directory." << std::endl;
        return 1;
    }

    for (boost::filesystem::directory_iterator it(datasetFolderPath); it != boost::filesystem::directory_iterator(); ++it) {
        if (boost::filesystem::is_regular_file(*it)) {
                std::string fileName = it->path().string();
        load_data("output.txt");
        ETL etl(argv[1], argv[2], argv[3]);

        std::vector<std::vector<std::string>> dataset = etl.readCSV();
        float peak_voltage = (float)std::get<0>(getMaxValue(dataset));
        float max_current = atof(dataset[std::get<1>(getMaxValue(dataset))][0].c_str());

        int rows = dataset.size();
        int cols = dataset[0].size();

        Eigen::MatrixXd dataMat = etl.CSVtoEigen(dataset, rows, cols);

        Eigen::MatrixXd standardDeviation = etl.Std(dataMat);
        Eigen::MatrixXd mean = etl.Mean(dataMat);
        std::cout << dataMat << std::endl;

        std::cout << "standard deviation" << standardDeviation << std::endl;
        std::cout << "mean:" << mean << std::endl;
        std::cout << "max voltage: " << peak_voltage << std::endl;
        std::cout << "corresponding current to max voltage: " << max_current << std::endl;

        std::vector<double> values = extractValues(dataset, 1); // Assuming column 1 is the relevant column
        double mu = std::get<0>(calculateMeanAndSigma(values));
        double sigma = std::get<1>(calculateMeanAndSigma(values));
        // double gaussian_peak_value = std::get<0>(getGaussianPeak(mu, sigma));
        // double peak_x = std::get<1>(getGaussianPeak(mu, sigma));
        // another way of getting values from a tuple
        double gaussian_peak_value, peak_x;
        std::tie(gaussian_peak_value, peak_x) = getGaussianPeak(mu, sigma, peak_voltage);
        float peak_current = atof(dataset[peak_x][0].c_str());

        std::cout << "Gaussian fit mean (mu): " << mu << ", standard deviation (sigma): " << sigma << std::endl;
        std::cout << "Gaussian peak value is: " << gaussian_peak_value << ", occurring at x = " << peak_current << std::endl;
        std::cout << "Better approximation:" << (peak_current + max_current) / 2 << std::endl;

        // storing the maximum value for the dirct method and the guassian fit
        std::vector<std::pair<double, double>> dir_max_and_x_value;
        dir_max_and_x_value.push_back(std::make_pair(peak_voltage, max_current));
        appenDataToAllDataSets(peak_voltage,max_current,all_direct_data);
        std::vector<std::pair<double, double>> guassian_max_and_x_value;
        guassian_max_and_x_value.push_back(std::make_pair(gaussian_peak_value, peak_current));
        appenDataToAllDataSets(gaussian_peak_value,peak_current,all_gauss_data);
            // Output to console (optional)
            for (const auto& pair : guassian_max_and_x_value) {
                std::cout << pair.first << ", " << pair.second << std::endl;
            }
            save_data("dirMaxOutput.txt", dir_max_and_x_value);
            save_data("Gaussoutput.txt", guassian_max_and_x_value);
            }
    } // end of for loop through all files in the dataset folder
    return EXIT_SUCCESS;
}

