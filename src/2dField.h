#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <vector>
#include <functional>

#include "FFT.h"
#include "GridObject.h"

typedef unsigned int uint;

class 2dField
{
    private:
        std::vector<double> K2;
        std::vector<double> kappa;

        void init_field(std::function<void(2dField &, uint, uint)> init_fcn);

    public:
        uint nx, ny;
        GridObject f1;
        GridObject f2;
        GridObject f3;

        double total_U;

        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        2dField(); //TODO: see if this can be removed
        2dField(uint nx, uint ny, double dx, double dy,
            std::function<void(2dField &, uint, uint)> init_fcn);
        ~2dField();
        //-----------------------------------------

        int solve_field_spectral(std::vector<double> re, std::vector<double> im);
        // solve_field_spectral may not currently work
        int solve_field(GridObject *charge_density);
        void print_field();
};

#endif
