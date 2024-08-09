#include "./ETL/ETL.h"
#include <iostream>
#include <string>
#include <Eigen/Dense>
#include <boost/algorithm/string.hpp>
#include <vector>
#include "calcMax.h"
#include "gausianFitMax.h"

int main(int argc, char *argv[])
{

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
     std::cout<< "max voltage: " << peak_voltage << std::endl;
     std::cout<< "corresponding current to max voltage: " << max_current << std::endl;
 
   std::vector<double> values = extractValues(dataset, 1); // Assuming column 1 is the relevant column
     double mu = std::get<0>(calculateMeanAndSigma(values));
    double sigma = std::get<1>(calculateMeanAndSigma(values));
    // double gaussian_peak_value = std::get<0>(getGaussianPeak(mu, sigma));
    // double peak_x = std::get<1>(getGaussianPeak(mu, sigma));
    //another way of getting values from a tuple
    double gaussian_peak_value , peak_x;
    std::tie( gaussian_peak_value, peak_x) =getGaussianPeak(mu, sigma,peak_voltage);
    float peak_current = atof(dataset[peak_x][0].c_str());

    std::cout << "Gaussian fit mean (mu): " << mu << ", standard deviation (sigma): " << sigma << std::endl;
    std::cout << "Gaussian peak value is: " << gaussian_peak_value << ", occurring at x = " << peak_current << std::endl;
    return EXIT_SUCCESS;
}
