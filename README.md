# Black-Scholes FEM Solver: A Computational Approach to Options Pricing

Inspired by Veritasium's "The Trillion Dollar Equation," this project bridges continuous-time financial mathematics with high-performance numerical engineering. While the Black-Scholes-Merton (BSM) model is traditionally solved via Finite Difference Methods (FDM) or Monte Carlo, this repository implements a highly optimized **1D Finite Element Method (FEM)** engine in C++ to solve the parabolic partial differential equation.

## 🚀 Engine Architecture & Current Implementation

The core computational engine and mathematical transformations are fully implemented:

* **Sparse Matrix Architecture:** Built in C++ using `Eigen3`. Utilizes compressed sparse column (CSC) formats and the `Eigen::SparseLU` solver for memory-efficient linear algebra.
* **Unconditionally Stable Time-Stepping:** Implements a second-order **Crank-Nicolson** integration loop, bypassing standard CFL condition limits for temporal stability.
* **Zero-Overhead Boundary Enforcement:** Dynamically enforces Dirichlet boundary conditions at each time step via the Penalty Method ($10^{15}$ scalar injection), perfectly preserving the tridiagonal matrix layout.
* **Mathematical Transformation:** To avoid the "moving boundary problem," the engine transforms the variable-coefficient Black-Scholes PDE into the canonical, constant-coefficient Heat Equation ($u_\tau = u_{xx}$). *For the full algebraic derivation and dimensional analysis of this substitution, please refer to the detailed mathematical report included in this repository.*

## 🗺️ Future Roadmap & Quantitative Analysis

With the solver mathematically verified, the next phase focuses on rigorous error bounding and extending financial utility:

* **Convergence Analysis:** Automating the calculation of $L_2$ (Root Mean Square) and $L_\infty$ (Supremum) error norms to mathematically prove the $O(\Delta x^2) + O(\Delta t^2)$ convergence rates.
* **Strike Price Singularity (The "Kink"):** The standard European Call payoff $\max(S-K, 0)$ introduces a discontinuous first derivative, causing spurious Crank-Nicolson oscillations (Gibbs phenomenon). Future commits will mitigate this via **Rannacher time-stepping** and **non-uniform nodal clustering**.
* **Analytical Benchmarking:** Integrating a continuous closed-form BSM benchmark ($N(d_1), N(d_2)$) to dynamically track the numerical solver's absolute and relative errors.
* **Extraction of The Greeks:** Leveraging the FEM shape functions to compute risk sensitivities directly from the pricing surface (e.g., extracting Delta $\Delta$ and Gamma $\Gamma$ via the first and second spatial derivatives).
