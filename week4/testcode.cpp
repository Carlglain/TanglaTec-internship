#include <iostream>
#include <vector>
int main(){
	std::vector<std::string> names={"storm","carl"};
	std::cout<<names[0];
	// Create a 3x3 matrix using Eigen's MatrixXd type
    Eigen::MatrixXd A(3, 3);

    // Initialize the matrix with values
    A << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

    // Create a 3x1 vector using Eigen's VectorXd type
    Eigen::VectorXd b(3);

    // Initialize the vector with values
    b << 10, 11, 12;

    // Solve the linear system Ax = b using Eigen's LU decomposition
    Eigen::VectorXd x = A.lu().solve(b);

    // Print the solution
    std::cout << "Solution: " << x << std::endl;
	return 0;
}