#include "./ETL/ETL.h"
#include <iostream>
#include <string>
#include <Eigen/Dense>
#include <vector>
#include <fstream>
#include <windows.h>
#include "gausianFitMax.h"
#include "calcMax.h"
#include <cstdlib>
#include <stdexcept>

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

void appendDataToAllDataSets(double max_value, double peak_pos, std::vector<std::vector<std::pair<double, double>>>& library) {
    std::vector<std::pair<double, double>> current_data;
    current_data.push_back(std::make_pair(max_value, peak_pos));
    library.push_back(current_data);
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <dataset_folder> <delimiter> <header>" << std::endl;
        return 1;
    }

    std::string datasetFolder = argv[1];
    std::string delimiter = argv[2];
    bool header = (std::string(argv[3]) == "True");

    std::cout << "Dataset Folder: " << datasetFolder << std::endl;
    std::cout << "Delimiter: " << delimiter << std::endl;
    std::cout << "Header: " << (header ? "True" : "False") << std::endl;

    std::string search_path = datasetFolder + "\\*";
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(search_path.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Dataset folder '" << datasetFolder << "' does not exist or is not a directory." << std::endl;
        return 1;
    }

    do {
        if (std::string(findFileData.cFileName) == "." || std::string(findFileData.cFileName) == "..") {
            continue;
        }

        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            std::string fileName = datasetFolder + "\\" + findFileData.cFileName;
            load_data(fileName);
            ETL etl(fileName, delimiter, header);

            std::vector<std::vector<std::string>> dataset = etl.readCSV();
            if (dataset.empty() || dataset[0].empty()) {
                std::cerr << "Error: Dataset is empty or malformed in file " << fileName << std::endl;
                continue;
            }

            auto maxValueResult = getMaxValue(dataset);
            float peak_voltage = std::get<0>(maxValueResult);
            float max_current = std::stof(dataset[std::get<1>(maxValueResult)][0]);

            int rows = dataset.size();
            int cols = dataset[0].size();

            Eigen::MatrixXd dataMat = etl.CSVtoEigen(dataset, rows, cols);

            Eigen::MatrixXd standardDeviation = etl.Std(dataMat);
            Eigen::MatrixXd mean = etl.Mean(dataMat);
            //std::cout << dataMat << std::endl;

            std::cout << "Standard deviation: " << standardDeviation << std::endl;
            std::cout << "Mean: " << mean << std::endl;
            std::cout << "Max voltage: " << peak_voltage << std::endl;
            std::cout << "Corresponding current to max voltage: " << max_current << std::endl;

            std::vector<double> values = extractValues(dataset, 1);
            auto meanAndSigma = calculateMeanAndSigma(values);
            double mu = std::get<0>(meanAndSigma);
            double sigma = std::get<1>(meanAndSigma);

            double gaussian_peak_value, peak_x;
            std::tie(gaussian_peak_value, peak_x) = getGaussianPeak(mu, sigma, peak_voltage);
            float peak_current = std::stof(dataset[peak_x][0]);

            std::cout << "Gaussian fit mean (mu): " << mu << ", standard deviation (sigma): " << sigma << std::endl;
            std::cout << "Gaussian peak value is: " << gaussian_peak_value << ", occurring at x = " << peak_current << std::endl;
            std::cout << "Better approximation: " << (peak_current + max_current) / 2 << std::endl;

            std::vector<std::pair<double, double>> dir_max_and_x_value;
            dir_max_and_x_value.push_back(std::make_pair(peak_voltage, max_current));
            appendDataToAllDataSets(peak_voltage, max_current, all_direct_data);

            std::vector<std::pair<double, double>> guassian_max_and_x_value;
            guassian_max_and_x_value.push_back(std::make_pair(gaussian_peak_value, peak_current));
            appendDataToAllDataSets(gaussian_peak_value, peak_current, all_gauss_data);

            for (const auto& pair : guassian_max_and_x_value) {
                std::cout << pair.first << ", " << pair.second << std::endl;
            }

            save_data("dirMaxOutput.txt", dir_max_and_x_value);
            save_data("Gaussoutput.txt", guassian_max_and_x_value);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return EXIT_SUCCESS;
}
