#include <iostream>
#include <vector>
#include <fstream>
#include <Eigen/Sparse>
#include <Eigen/Dense>

#include "mesh_generator.h"
#include "systemAssembly.h"
#include "solver.h"
#include "EuropeanOptions.h"

using namespace std;

int main() {
    double k, t, rr, sig, t_int, smax, smin;
    int n;
    
    // Note: Make sure to prompt the user or know what order these are in!
    cin >> n >> smax >> smin >> k >> rr >> sig >> t >> t_int;

    CallOption model(k, t, rr, sig, t_int, n, smax, smin);

    std::vector<std::pair<double,Eigen::VectorXd>> results = model.FairPriceGen();
    
    // Grab the actual stock prices from the engine safely!
    std::vector<double> S_grid = model.getStockPrices();

    // Export to CSV
    std::ofstream outFile("solver_output.csv");

    if (outFile.is_open()) {
        // 1. Write the Stock Prices as the header row
        outFile << "Time_to_Maturity";
        for(int i = 0; i < n; i++) {
            outFile << "," << S_grid[i]; // Prints the exact $S$ value!
        }
        outFile << "\n";

        // 2. Loop through your results and print the time and the vector values
        for (const auto& step : results) {
            outFile << step.first; // Print the time
            
            // Print all option values at that time
            for (int i = 0; i < step.second.size(); i++) {
                outFile << "," << step.second(i);
            }
            outFile << "\n";
        }
        outFile.close();
        std::cout << "Successfully wrote output to solver_output.csv!" << std::endl;
    } else {
        std::cout << "Error: Could not open file for writing." << std::endl;
    }

    return 0;
}