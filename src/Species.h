#ifndef SPECIES_H
#define SPECIES_H

#include <iostream>
#include <vector>
#include <functional>

#include "GridObject.h"
#include "Particle.h"
#include "Field.h"
#include "ThreeVec.h"

class Species
{
    private:
        std::vector<Particle> parts;


        /**********************************************************
        PRIVATE CLASS METHODS
        ***********************************************************/
        void init_species(std::function<void(Species &, uint)> init_fcn);

        void apply_bc(ThreeVec &pos,
                      const double L_x, const double L_y,
                      const double dx, const double dy);
        //-----------------------------------------

    public:
        uint npar;

        double density;
        GridObject density_arr;

        double Qpar;

        // Diagnostics
        double total_KE;


        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        Species();
        Species(uint npar, uint nx, uint ny, double Qpar);
        Species(uint npar, uint nx, uint ny, double Qpar, double density,
			    std::function<void(Species &, uint)> init_fcn);

        ~Species();
        //-----------------------------------------


        /**********************************************************
        CLASS METHODS
        ***********************************************************/
        void add_particle(double x_pos, double y_pos, double z_pos,
                          double x_mom, double y_mom, double z_mom,
                          double Wpar);
        void add_particle(ThreeVec pos, ThreeVec mom, double Wpar);
        void add_particle(Particle p);

        int deposit_charge(const double dx, const double dy,
                           const double L_x, const double L_y,
                           const uint Nx, const uint Ny);

        int map_field_to_part(const Field &f,
                              const double dx, const double dy,
                              const double L_x, const double L_y,
                              const uint Nx, const uint Ny);

        int push_particles(const double L_x, const double L_y,
                           const double dt,
                           const double dx, const double dy);

        std::vector<double> get_x_phasespace();
        std::vector<double> get_y_phasespace();
        std::vector<double> get_px_phasespace();
        std::vector<double> get_py_phasespace();
        void print_part_v_coord(uint ii);
        void print_part_coord(uint ii);
        void print_E_x();
        void print_E_y();
        void print_E_z();
        void print_B_x();
        void print_B_y();
        void print_B_z();
        void print_weights();
        void print_density();
        //-----------------------------------------
};

#endif
