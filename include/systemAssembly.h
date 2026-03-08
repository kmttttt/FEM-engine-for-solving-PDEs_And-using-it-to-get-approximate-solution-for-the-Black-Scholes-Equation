#ifndef SYSTEMASSEMBLY_H
#define SYSTEMASSEMBLY_H

#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <vector>

class SystemAssembly {
    public :
        SystemAssembly(double r,double cp,double k);
        
        std::pair<Eigen::Matrix2d,Eigen::Matrix2d> ElementaryMatrix(double he);

        std::pair<Eigen::SparseMatrix<double> , Eigen::SparseMatrix<double>> GlobalAssembly(std::vector<double>& mesh);
    private :
        double rho,Cp,K;
        double alpha;
};
#endif // SYSTEMASSEMBLY_H
