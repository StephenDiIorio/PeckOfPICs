#ifndef SPECIES_H
#define SPECIES_H

#include <iostream>
#include <vector>
#include <functional>

#include "GridObject.h"
#include "Particle.h"
#include "Field.h"
#include "ThreeVec.h"

enum field_type {electric, magnetic};

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

        int map_field_to_part(const Field &f, field_type field_to_map,
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
        std::vector<double> get_local_E(int i);
        std::vector<double> get_local_E_x();
        std::vector<double> get_local_E_y();
        std::vector<double> get_local_E_z();
        std::vector<double> get_local_B(int i);
        std::vector<double> get_local_B_x();
        std::vector<double> get_local_B_y();
        std::vector<double> get_local_B_z();

        // Print Functions
        void print_pos() const;
        void print_pos_comp(uint i) const;
        void print_mom() const;
        void print_mom_comp(uint i) const;
        void print_weight() const;
        void print_local_e_field() const;
        void print_local_e_field_comp(uint i) const;
        void print_local_b_field() const;
        void print_local_b_field_comp(uint i) const;
        void print_density() const;
        //-----------------------------------------
};

#endif
