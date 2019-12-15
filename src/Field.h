#ifndef Field_H
#define Field_H

#include <iostream>
#include <vector>
#include <functional>

// #include <fftw3.h>

#include "FFT.h"
#include "GridObject.h"

// namespace Field_T
// {
    const char Field_T_err[31] = "Error: Field type is undefined";
    enum Field_Type
    {
        Electric,
        Magnetic
    };
// }

class Field
{
    private:
        enum accessors
        {
            x1_accessor = 0,
            x2_accessor = 1,
            x3_accessor = 2
        };

        //TODO: these should just be defined once for all fields rather than for each field object
        std::vector<double> K_x2, K_y2;
        std::vector<double> Kappa_x, Kappa_y;
        GridObject phi_dens_re, phi_dens_im, Ex_im, Ey_im;

        char no_dimension_err[45] = "Error: Field dimension does not exist"; //TODO: want to make this constant but it destroys the assignment constructor. need to define my own operator= ?


        /**********************************************************
        PRIVATE CLASS METHODS
        ***********************************************************/
        void init_field(std::function<void(Field &, std::size_t, std::size_t)> init_fcn, std::size_t Nx, std::size_t Ny);
        //-----------------------------------------


        /**********************************************************
        TO BE IMPLEMENTED
        ***********************************************************/
        // solve_field_spectral may not currently work
        int solve_field_spectral(std::vector<double> re, std::vector<double> im);
        int solve_field_fftw(const GridObject& charge_density);
        //-----------------------------------------

    public:
        GridObject f1;
        GridObject f2;
        GridObject f3;

        double total_U;

        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        Field(); //TODO: see if this can be removed
        Field(const std::size_t Nx, const std::size_t Ny,
              const double dx, const double dy); // zero constructor
        Field(const std::size_t Nx, const std::size_t Ny,
              const double dx, const double dy,
              std::size_t component, double value); // constant constructor
        Field(const std::size_t Nx, const std::size_t Ny,
              const double dx, const double dy,
              std::function<void(Field &, std::size_t, std::size_t)> init_fcn);
        ~Field();
        //-----------------------------------------


        /**********************************************************
        CLASS METHODS
        ***********************************************************/
        int solve_field(const GridObject& charge_density,
                        const double dx, const double dy);

        void print_field();
        //-----------------------------------------
};

#endif
