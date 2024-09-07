#include <iostream>
#include <vector>
#include <utility> // for std::pair

// Function to store max value and its x/y coordinates in the library
void appendDataToAllDataSets(double max_value, double x_value, double y_value, 
                              std::vector<std::vector<std::pair<double, double>>>& library) {
    // Create a vector for the current data
    std::vector<std::pair<double, double>> current_data;
    
    // Append pairs (x_value, max_value) and (y_value, max_value)
    current_data.push_back(std::make_pair(x_value, max_value));
    current_data.push_back(std::make_pair(y_value, max_value));
    
    // Store the current data in the library
    library.push_back(current_data);
}

int main() {
    std::vector<std::vector<std::pair<double, double>>> library; // Data storage

    double max_value = 25.1; // Example maximum value
    double x_value = 1.2;     // Example x value
    double y_value = 2.5;     // Example y value

    appendDataToAllDataSets(max_value, x_value, y_value, library);

    // Output the contents of the library
    for (const auto& data_set : library) {
        for (const auto& pair : data_set) {
            std::cout << "(" << pair.first << ", " << pair.second << ") ";
        }
        std::cout << std::endl;
    }

    return 0;
}