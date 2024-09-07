#include "ETL.h"

#include <vector>
#include <stdlib.h>
#include <cmath>
#include <boost/algorithm/string.hpp>

std::vector<std::vector<std::string>> ETL::readCSV()
{

    std::ifstream file(dataset);
    std::vector<std::vector<std::string>> dataString;

    std::string line = "";

    while (getline(file, line)) {
        std::vector<std::string> vec;
        boost::algorithm::split(vec, line, boost::is_any_of(delimiter));
        dataString.push_back(vec);
    }

    file.close();

    return dataString;
}

Eigen::MatrixXd ETL::CSVtoEigen(std::vector<std::vector<std::string>> dataset, int rows, int cols)
{

    if (header == true)
    {
        rows = rows - 1;
    }

    Eigen::MatrixXd mat(cols, rows);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; ++j)
        {
            mat(j, i) = atof(dataset[i][j].c_str());
        }
    }

    return mat.transpose();
}

auto ETL::Mean(Eigen::MatrixXd data) -> decltype(data.colwise().mean())
{
    return data.colwise().mean();
}	

auto ETL::Std(Eigen::MatrixXd data) -> decltype(((data.array().square().colwise().sum()) / (data.rows() - 1)).sqrt())
{
    return ((data.array().square().colwise().sum()) / (data.rows() - 1)).sqrt();
}

Eigen::MatrixXd ETL::Normalize(Eigen::MatrixXd data, bool normalizeTarget){

    Eigen::MatrixXd dataNorm;
    if (normalizeTarget == true)
    {
        return dataNorm = data;
    }
    else
    {
       return dataNorm = data.leftCols(data.cols() - 1);
    }
}