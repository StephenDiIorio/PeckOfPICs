/*
    Another option, to make it more modular from different simulation settings, is to have init_sim take in just a simulation object and create a new function, new_sim. init_sim would work on parsing all of the parameters for the simulation, and new_sim would be called at the end of init_sim and would handle assigning the values of the simulation object for the specific problem. Then, in the main function, call init_sim. Can then modularize this by either including a separate file that handles all of the parameter read in and initialization.
*/

#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <functional>

#include "GridObject.h"
#include "Species.h"
#include "Field.h"

class Simulation
{
    private:
        int err;


        /**********************************************************
        CLASS METHODS
        ***********************************************************/
        void init_simulation();

        void deposit_charge();
        void solve_field();
        void map_field_to_species();
        void push_species();
        //-----------------------------------------

    public:
        unsigned long n_iter;
        uint ndump;

        // Grid information
        uint Nx;     // number of grid points in x
        uint Ny;     // number of grid points in y
        double L_x;  // system length in x
        double L_y;  // system length in y
        double dx;   // grid spacing in x
        double dy;   // grid spacing in y

        // Temporal information
        double dt;    // timestep
        double tmax;  // max time

        // Species information
        uint nspec;  // number of species
        std::vector<Species> spec;

        // Field information
        Field e_field;
        Field b_field;


        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        Simulation(uint ndump,
                   uint nx, uint ny,
                   double L_x, double L_y,
                   double dt, double tmax);
        ~Simulation();
        //-----------------------------------------


        /**********************************************************
        CLASS METHODS
        ***********************************************************/
        void add_species(uint npar, double Qpar, double density,
                         std::function<void(Species &, uint)> init_fcn);
        void add_e_field(std::function<void(Field &, uint)> init_fcn);
        void add_b_field(std::function<void(Field &, uint)> init_fcn);

        bool dump_data();
        void iterate();
        GridObject get_total_density();

        void print_spec_density(uint i);
        //-----------------------------------------
};

#endif
