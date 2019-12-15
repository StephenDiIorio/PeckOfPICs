#ifndef SPECIES_H
#define SPECIES_H

#include <iostream>
#include <vector>
#include <functional>

#include "GridObject.h"
#include "DataStorage_1D.h"
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
        void init_species(std::function<void(Species &, std::size_t)> init_fcn);

        void _apply_bc(ThreeVec& pos,
                       const double L_x, const double L_y,
                       const double dx, const double dy);
        //-----------------------------------------

    public:
        std::size_t Npar;

        GridObject density_arr;

	      double Qpar;

        // Diagnostics
        // double total_KE;


        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        Species();
        Species(std::size_t Npar, std::size_t Nx, std::size_t Ny, double Qpar);
        Species(std::size_t Npar, std::size_t Nx, std::size_t Ny, double Qpar,
			    std::function<void(Species &, std::size_t)> init_fcn);

	      ~Species();
	      //-----------------------------------------


        /**********************************************************
        CLASS METHODS
        ***********************************************************/
        void add_particle(double x_pos, double y_pos, double z_pos,
                          double x_mom, double y_mom, double z_mom,
                          double Wpar);
        void add_particle(const ThreeVec& pos, const ThreeVec& mom, double Wpar);
        void add_particle(const Particle& p);

        int deposit_charge(const double dx, const double dy,
                           const double L_x, const double L_y,
                           const std::size_t Nx, const std::size_t Ny);

        int map_field_to_part(const Field& f,
                              const Field_T::Field_Type field_to_map,
                              const double dx, const double dy,
                              const double L_x, const double L_y,
                              const std::size_t Nx, const std::size_t Ny);

        int push_particles(const double L_x, const double L_y,
                           const double dt,
                           const double dx, const double dy);

        void apply_bc(const double L_x, const double L_y,
                      const double dx, const double dy);

        DataStorage_1D get_x_phasespace();
        DataStorage_1D get_y_phasespace();
        DataStorage_1D get_px_phasespace();
        DataStorage_1D get_py_phasespace();
        std::vector<double> get_local_E(std::size_t i);
        std::vector<double> get_local_B(std::size_t i);

        // Print Functions
        void print_pos() const;
        void print_pos_comp(std::size_t i) const;
        void print_mom() const;
        void print_mom_comp(std::size_t i) const;
        void print_weight() const;
        void print_local_e_field() const;
        void print_local_e_field_comp(std::size_t i) const;
        void print_local_b_field() const;
        void print_local_b_field_comp(std::size_t i) const;
        void print_density() const;
        //-----------------------------------------
};

#endif
