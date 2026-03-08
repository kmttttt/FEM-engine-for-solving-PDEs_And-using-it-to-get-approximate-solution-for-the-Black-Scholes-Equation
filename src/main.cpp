#include <iostream>
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include "mesh_generator.h"
#include "systemAssembly.h"

int main() {
    // Define a sparse matrix of doubles
    Mesh1D mesh(1.0, 0.0, 10);
    auto uniform_mesh = mesh.generateUniform();
    std::cout << "Generated uniform mesh with " << uniform_mesh.size() << " points." << std::endl;

    SystemAssembly sys(1,1,1);
    auto [K,M] = sys.GlobalAssembly(uniform_mesh);
    cout << K << endl <<M;
    return 0;
}