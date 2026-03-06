#include <mesh_generator.h>
#include  <cmath>
Mesh1D::Mesh1D(double x_max, double x_min, int N) : x_max(x_max), x_min(x_min), num_elements(N) {}

vector<double> Mesh1D::generateUniform(){
    vector<double> mesh(num_elements+1);
    double dx = (x_max - x_min) / num_elements;
    for ( int i = 0; i< num_elements+1; i++){
        mesh[i] = x_min + (i)*dx;
    }
    return mesh;
}

vector<double> Mesh1D::generateNonUniform(double strike_x, double density_factor){
    double dx = (x_max - x_min) / num_elements;
    vector<double> mesh(num_elements+1);
    for ( int i = 0; i< num_elements+1; i++){
        double x = x_min + (i)*dx;
        if (x <= strike_x) mesh[i] = strike_x - (strike_x-x_min)*pow(abs(x - strike_x) / (strike_x - x_min), density_factor);
        else mesh[i] = strike_x + (x_max-strike_x)*pow(abs(x - strike_x) / (x_max - strike_x), density_factor);
    }
    return mesh;
}
   