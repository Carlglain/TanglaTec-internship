#include<vector>
#include <utility>
std::vector<std::pair<double, double>> max_values_and_positions;
void appendMaxValue(double maxValue,double x_value){
max_values_and_positions.push_back(std::make_pair(maxValue,x_value));

}