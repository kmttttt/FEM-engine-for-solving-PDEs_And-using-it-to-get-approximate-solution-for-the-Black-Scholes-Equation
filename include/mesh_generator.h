#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include <vector>

using namespace std;
class Mesh1D {
public:
    Mesh1D(double x_max, double x_min, int N);

    vector<double> generateUniform();
    vector<double> generateNonUniform(double strike_x, double density_factor);

private:
    double x_max;
    double x_min;
    int num_elements;
};

#endif // MESH_GENERATOR_H