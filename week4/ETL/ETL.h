// ETL : extract, transform and load.
// It will contain API to let us read data from a csv file

#ifndef ETL_h
#define ETL_h

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Eigen/Dense> //template library for linear algebra

// Class to hold the data read from the CSV file
class ETL
{

    std::string dataset;
    std::string delimiter;
    bool header; // header flag
public:
    ETL(std::string data, std::string separator, bool head) : dataset(data), delimiter(separator), header(head)
    {
    }
    std::vector<std::vector<std::string>> readCSV();
    Eigen::MatrixXd CSVtoEigen(std::vector<std::vector<std::string>> dataset, int rows, int cols);
    Eigen::MatrixXd Normalize(Eigen::MatrixXd data, bool normalizeTarget);
    auto Mean(Eigen::MatrixXd data) -> decltype(data.colwise().mean());
    auto Std(Eigen::MatrixXd data) -> decltype(((data.array().square().colwise().sum()) / (data.rows() - 1)).sqrt());
};

#endif