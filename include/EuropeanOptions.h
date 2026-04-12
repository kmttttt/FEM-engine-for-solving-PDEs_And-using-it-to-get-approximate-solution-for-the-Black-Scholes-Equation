#ifndef EUROPEANOPTIONS_H
#define EUROPEANOPTIONS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <Eigen/Sparse>
#include <Eigen/Dense>

#include "mesh_generator.h"
#include "systemAssembly.h"
#include "solver.h"

class CallOption{
    private :
        double K,T,r,sigma;

        double S_max;
        double S_min;
        double dtau;
        int num_nodes;

        double alpha,beta;

        Mesh1D mesh;
        SystemAssembly sys;
        vector<double> UniMesh,NonUniMesh;

    public:
        CallOption(double k, double t, double rr, double sig, double t_intv, int n, double Smax, double Smin);

        Eigen::VectorXd getPayoff();
        std::vector<std::pair<double,Eigen::VectorXd>> FairPriceGen();
        std::vector<double> getStockPrices() const;
};
#endif