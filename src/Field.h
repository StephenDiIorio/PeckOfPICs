#ifndef Field_H
#define Field_H

#include <iostream>
#include <vector>
#include <functional>

#include "FFT.h"
#include "GridObject.h"

typedef unsigned int uint;

class Field
{
    private:
        std::vector<double> K2;
        std::vector<double> kappa;

        void init_field(std::function<void(Field &, uint, uint)> init_fcn);

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
        Field(); //TODO: see if this can be removed
        Field(uint nx, uint ny, double dx, double dy,
            std::function<void(Field &, uint, uint)> init_fcn);
        ~Field();
        //-----------------------------------------

        int solve_field_spectral(std::vector<double> re, std::vector<double> im);
        // solve_field_spectral may not currently work
        int solve_field(GridObject *charge_density);
        void print_field();
};

#endif
