#ifndef FIELD2d_H
#define FIELD2d_H

#include <iostream>
#include <vector>
#include <functional>

#include "FFT.h"
#include "GridObject.h"

typedef unsigned int uint;

class Field2d
{
    private:
        std::vector<double> K2;
        std::vector<double> kappa;

        void init_field(std::function<void(Field2d &, uint, uint)> init_fcn);
        GridObject density_bar;

    public:
        uint nx, ny;
        uint size;
        GridObject f1;
        GridObject f2;
        GridObject f3;

        double total_U;

        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        Field2d(); //TODO: see if this can be removed
        Field2d(uint nx, uint ny, double dx, double dy,
            std::function<void(Field2d &, uint, uint)> init_fcn);
        ~Field2d();
        //-----------------------------------------

        int solve_field_spectral(GridObject charge_density);
        // solve_field_spectral may not currently work
        int solve_field(GridObject charge_density);
        void print_field();
};

#endif
