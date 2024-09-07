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
#include <tuple>

std::vector<std::vector<std::tuple<double, double, double>>> all_gauss_data; // Store multiple dataset maximum values and their positions
std::vector<std::vector<std::tuple<double, double, double>>> all_direct_data;

void load_data(const std::string& fileLocation) {
    std::ifstream in_file(fileLocation);
    if (!in_file.is_open()) {
        std::cerr << "Error opening input file: " << fileLocation << std::endl;
        return;
    }
    in_file.close();
}

void saveToFile(const std::string& fileLocation, const std::vector<std::tuple<double, double, double>>& data) {
    std::ofstream out_file(fileLocation, std::ios::app);
    if (!out_file.is_open()) {
        std::cerr << "Error opening output file: " << fileLocation << std::endl;
        return;
    }
    // Iterate over the vector and save three variables
    for (const auto& entry : data) {
        out_file << std::get<0>(entry) << "," // Max value
                 << std::get<1>(entry) << "," // X value
                 << std::get<2>(entry) << std::endl; // Y value
    }
    out_file.close();
}

void appendDataToAllDataSets(double max_value, double peak_pos_x, double peak_pos_y, 
                              std::vector<std::vector<std::tuple<double, double, double>>>& library) {
    // Directly append the tuple to the library
    library.emplace_back(std::vector<std::tuple<double, double, double>>{std::make_tuple(max_value, peak_pos_x, peak_pos_y)});
}

int main(int argc, char* argv[]) {
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
            float max_x_value = std::stof(dataset[std::get<1>(maxValueResult)][0]);
            float max_y_value = std::stof(dataset[std::get<1>(maxValueResult)][1]);

            Eigen::MatrixXd dataMat = etl.CSVtoEigen(dataset, dataset.size(), 3);
            std::cout << "Max voltage: " << peak_voltage << std::endl;
            std::cout << "X position where this maximum occurs: " << max_x_value << std::endl;
            std::cout << "Y position where this maximum occurs: " << max_y_value << std::endl;

            double gaussian_peak_value, peak_x, peak_y;
            std::tie(peak_x, peak_y, gaussian_peak_value) = findGaussianPeak(dataset, 0, 1, 2);
            std::cout << "Gaussian peak value is: " << gaussian_peak_value 
                      << ", occurring at x = " << peak_x 
                      << " and y = " << peak_y << std::endl;

            // Append data to all datasets
            appendDataToAllDataSets(peak_voltage, max_x_value, max_y_value, all_direct_data);
            appendDataToAllDataSets(gaussian_peak_value, peak_x, peak_y, all_gauss_data);

            // Print Gaussian data
            const auto& latestGaussianData = all_gauss_data.back();
            for (const auto& entry : latestGaussianData) {
                std::cout << std::get<0>(entry) << ", " // Max value
                          << std::get<1>(entry) << ", " // X value
                          << std::get<2>(entry) << std::endl; // Y value
            }

            // Save the latest entries
            saveToFile("dirMaxOutput.txt", all_direct_data.back());
            saveToFile("Gaussoutput.txt", latestGaussianData);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return EXIT_SUCCESS;
}