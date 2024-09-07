#include "ETL.h"

#include <vector>
#include <stdlib.h>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <cstdlib> // for atof

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

Eigen::MatrixXd ETL::CSVtoEigen(std::vector<std::vector<std::string>> dataset, int rows, int cols )
{

	
    if (header) {
        if (rows > 0) {
            rows--; // Adjust row count if there is a header
        } else {
            return Eigen::MatrixXd(); // Return an empty matrix if dataset is empty
        }
    }

    Eigen::MatrixXd mat(rows, cols); // Matrix with rows for points and 3 columns for (x, y, z)

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; ++j) {
            // Adjust index if there's a header
            int index = i + (header ? 1 : 0);
            if (index < dataset.size() && j < dataset[index].size()) {
                mat(i, j) = atof(dataset[index][j].c_str()); // Convert string to double
            } else {
                mat(i, j) = 0.0; // Default value if data is missing (handle error gracefully)
            }
        }
    }

    return mat; // Return the matrix
}

