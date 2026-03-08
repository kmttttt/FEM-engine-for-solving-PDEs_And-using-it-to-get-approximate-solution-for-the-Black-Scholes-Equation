#include <systemAssembly.h>


SystemAssembly::SystemAssembly(double r,double cp,double k){
    rho = r;
    Cp = cp;
    K = k;
    alpha = K/(rho*Cp);
}

std::pair<Eigen::Matrix2d,Eigen::Matrix2d> SystemAssembly::ElementaryMatrix(double he){
    Eigen::Matrix2d Ke,Me;
    for(int i = 0 ; i<2; i++){                  // Ke << (alpha/he),(-alpha/he),(-alpha/he),(alpha/he);
        for(int j = 0; j<2; j++){
            if(i==j) Ke(i,j) = alpha/he;
            else Ke(i,j) = -alpha/he;
        }
    }
    for(int i = 0 ; i<2; i++){                  // Me << he/6*((2),(1),(1),(2));
        for(int j = 0; j<2; j++){
            if(i==j) Me(i,j) = he/6*2;
            else Me(i,j) = he/6;
        }
    }
    return {Ke,Me};
}

std::pair<Eigen::SparseMatrix<double> , Eigen::SparseMatrix<double>> SystemAssembly::GlobalAssembly(std::vector<double>& mesh){
    long long n = mesh.size()-1;
    Eigen::SparseMatrix<double> K(n+1,n+1),M(n+1,n+1);
    for(int e = 0; e<n ; e++){
        auto [Ke,Me] = ElementaryMatrix(mesh[e+1]-mesh[e]);
        for(int i =0; i<2;i++){
            for(int j = 0; j<2;j++){
                K.coeffRef(e+i,e+j) += Ke(i,j);
                M.coeffRef(e+i,e+j) += Me(i,j);
            }
        }
    }
    return {K,M};
}