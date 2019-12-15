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
        PRIVATE CLASS METHODS
        ***********************************************************/
        void init_simulation();

        void _deposit_charge();
        void _solve_field();
        void _map_field_to_species();
        void _push_species();
        //-----------------------------------------

    public:
        std::size_t n_iter;
        std::size_t ndump;

        // Grid information
        std::size_t Nx;     // number of grid points in x
        std::size_t Ny;     // number of grid points in y
        double L_x;  // system length in x
        double L_y;  // system length in y
        double dx;   // grid spacing in x
        double dy;   // grid spacing in y

        // Temporal information
        double dt;    // timestep
        double tmax;  // max time

        // Species information
        std::size_t nspec;  // number of species
        std::vector<Species> spec;

        // Field information
        Field e_field;
        Field b_field;


        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        Simulation(std::size_t ndump,
                   std::size_t Nx, std::size_t Ny,
                   double L_x, double L_y,
                   double dt, double tmax);
        ~Simulation();
        //-----------------------------------------


        /**********************************************************
        CLASS METHODS
        ***********************************************************/
        void add_species(std::size_t npar, double Qpar,
                         std::function<void(Species&, std::size_t)> init_fcn);
        void add_e_field(std::function<void(Field&, std::size_t, std::size_t)> init_fcn);
        void add_b_field(std::function<void(Field&, std::size_t, std::size_t)> init_fcn);

        bool dump_data();
        void iterate();
        GridObject get_total_density();

        void print_spec_density(std::size_t i) const;
        //-----------------------------------------
};

#endif
