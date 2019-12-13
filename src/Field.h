#ifndef Field_H
#define Field_H

#include <iostream>
#include <vector>
#include <functional>

#include <fftw3.h>

#include "FFT.h"
#include "GridObject.h"

typedef unsigned int uint;

class Field
{
    private:
        std::vector<double> K2;
        std::vector<double> kappa;
        GridObject phi_dens_re, phi_dens_im, Ex_im, Ey_im;


        void init_field(std::function<void(Field &, uint, uint)> init_fcn);
        

    public:
        uint Nx, Ny;
        double dx, dy;
        uint size;
        GridObject f1;
        GridObject f2;
        GridObject f3;

        double total_U;

        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        Field(); //TODO: see if this can be removed
        Field(uint Nx, uint Ny, double dx, double dy); // zero constructor
        Field(uint Nx, uint Ny, double dx, double dy,\
            uint component, double value); // constant constructor
        Field(uint Nx, uint Ny, double dx, double dy,
            std::function<void(Field &, uint, uint)> init_fcn);
        ~Field();
        //-----------------------------------------

        int FFT_2d(GridObject &real_part, GridObject &imag_part, 
                        const uint transform_direction);

        int solve_field_spectral(std::vector<double> re, std::vector<double> im);
        // solve_field_spectral may not currently work
        int solve_field(const GridObject &charge_density);
        int solve_field_fftw(GridObject &charge_density);
        void print_field();
};

#endif
