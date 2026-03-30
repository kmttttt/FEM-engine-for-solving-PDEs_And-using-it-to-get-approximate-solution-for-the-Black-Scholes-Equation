#include <iostream>
#include <vector>
#include <fstream>
#include <Eigen/Sparse>
#include <Eigen/Dense>

#include "mesh_generator.h"
#include "systemAssembly.h"
#include "solver.h"

int main() {
    // 1. Generate the Mesh
    Mesh1D mesh(1.0, 0.0, 10); 
    auto non_uniform_mesh = mesh.generateUniform();
    
    // Store the number of nodes so we can use it to size our vectors
    int num_nodes = non_uniform_mesh.size();
    std::cout << "Generated mesh with " << num_nodes << " points." << std::endl;

    // 2. Assemble Global Matrices
    SystemAssembly sys(1.0, 1.0, 1.0);
    auto [K, M] = sys.GlobalAssembly(non_uniform_mesh);
    
    // Print out K and M to visually verify the tridiagonal structure
    std::cout << "\nGlobal Stiffness Matrix K:\n" << K << std::endl;
    std::cout << "Global Mass Matrix M:\n" << M << std::endl;

    // 3. Set up the Initial Condition (CRITICAL FIX)
    Eigen::VectorXd T_initial(num_nodes);
    T_initial.setZero(); // This represents a cold 1D rod at 0 degrees everywhere

    // 4. Initialize and Run the Solver
    solverCN mySolver(K, M);
    
    // Test parameters: Left edge clamped at 0, Right edge heated to 100
    double b_left = 0.0;
    double b_right = 100.0;
    double dt = 0.01;
    double total_time = 2.0;

    std::cout << "\nStarting Crank-Nicolson Solver..." << std::endl;
    std::vector<std::pair<double, Eigen::VectorXd>> results = mySolver.Crank_Nicolson(b_left, b_right, dt, total_time, T_initial);
    std::cout << "Solver finished successfully! Total steps taken: " << results.size() << std::endl;

    // 5. Export to CSV
    std::ofstream outFile("solver_output.csv");

    if (outFile.is_open()) {
        // Write a header row (Time, Node0, Node1, Node2...)
        outFile << "Time";
        for(int i = 0; i < num_nodes; i++) {
            outFile << ",Node_" << i;
        }
        outFile << "\n";

        // Loop through your results and print the time and the vector values
        for (const auto& step : results) {
            outFile << step.first; // Print the time
            
            // Print all temperatures at that time
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