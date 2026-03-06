#include <iostream>
#include <Eigen/Sparse>

int main() {
    // Define a sparse matrix of doubles
    Eigen::SparseMatrix<double> K(1000, 1000);
    
    std::cout << "Successfully integrated Eigen!" << std::endl;
    std::cout << "Global Stiffness Matrix K dimensions: " 
              << K.rows() << " x " << K.cols() << std::endl;
              
    return 0;
}