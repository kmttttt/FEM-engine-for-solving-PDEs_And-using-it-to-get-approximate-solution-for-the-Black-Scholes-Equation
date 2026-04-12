#include "EuropeanOptions.h"


CallOption::CallOption(double k, double t, double rr, double sig, double t_intv, int n, double Smax, double Smin) : num_nodes(n),K(k),sys(1.0,1.0,1.0),mesh(std::log(Smax/k),std::log(std::max(Smin,1e-4)/k),n-1) {
    T = t;
    r = rr;
    sigma = sig;
    dtau = t_intv*sigma*sigma/2.0;
    S_max = Smax;
    S_min = Smin;

    alpha = r/(sigma*sigma)-0.5;
    beta = (1.0+alpha)*(1.0+alpha);

    UniMesh = mesh.generateUniform();
    NonUniMesh = mesh.generateNonUniform(0.0,3);


}

Eigen::VectorXd CallOption::getPayoff(){
    Eigen::VectorXd Uo(num_nodes);
    double s,v;

    for(int i = 0; i<num_nodes; i++){
        s = K*std::exp(NonUniMesh[i]);
        v = std::max(s-K,0.0);
        Uo(i) = v;
    }
    return Uo;
}

std::vector<std::pair<double,Eigen::VectorXd>> CallOption::FairPriceGen(){
    std::pair<Eigen::SparseMatrix<double> , Eigen::SparseMatrix<double>> KM = sys.GlobalAssembly(NonUniMesh);
    Eigen::VectorXd initial_payoff = getPayoff();
    double tau_max = sigma*sigma/2.0*T;
    int n = KM.first.rows()-1;
    Eigen::VectorXd T1(num_nodes); // T_n+1
    T1.setZero();
    double time = 0;        //initially time is zero

    Eigen::SparseMatrix<double> A,B;
    Eigen::VectorXd F;
    A = KM.second + (dtau/2.0)*KM.first;
    B = KM.second - (dtau/2.0)*KM.first;
    A.coeffRef(0,0) = 1e15;
    A.coeffRef(n,n) = 1e15;

    A.makeCompressed();
    B.makeCompressed();

    std::vector<std::pair<double,Eigen::VectorXd>> outP;
    outP.push_back({T,initial_payoff});
    for(int j = 0; j<num_nodes; j++){
        initial_payoff(j) = initial_payoff(j)/(K*std::exp(-alpha*NonUniMesh[j]));
    }

    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
    solver.compute(A);

    double b_val2;
    Eigen::VectorXd V_St(num_nodes);

    int n_t = std::round(tau_max/dtau);
    for(int i = 1; i<= n_t; i++){
        time = i*dtau;
        b_val2 = (S_max-K*std::exp(-2.0*r*time/(sigma*sigma)))/K*std::exp(alpha*std::log(S_max/K) +beta*time);
        F = B*initial_payoff;
        F(0) = 1e15*0.0;
        F(n) = 1e15*b_val2;

        T1 = solver.solve(F);
        if(solver.info() != Eigen::Success) break;
        initial_payoff = T1;

        for(int j = 0; j<num_nodes; j++){
            V_St(j) = K*std::exp(-alpha*NonUniMesh[j] - beta*time)*initial_payoff(j);
        }
        outP.push_back({std::max(0.0,T-2.0*time/(sigma*sigma)),V_St});
    }

    return outP;
}

std::vector<double> CallOption::getStockPrices() const {
    std::vector<double> S_grid(num_nodes);
    for(int i = 0; i < num_nodes; i++) {
        S_grid[i] = K * std::exp(NonUniMesh[i]);
    }
    return S_grid;
}