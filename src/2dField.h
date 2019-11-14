#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <vector>
#include <functional>

#include "FFT.h"

typedef unsigned int uint;

class 2dField
{
    private:
        std::vector<double> K2;
        std::vector<double> kappa;

        void init_field(std::function<void(2dField &, uint)> init_fcn);

    public:
        uint size;
        std::vector<double> f1;
        std::vector<double> f2;
        std::vector<double> f3;

        double total_U;

        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        2dField(); //TODO: see if this can be removed
        2dField(uint nx, double dx, uint ny, double dy,
            std::function<void(Field &, uint)> init_fcn);
        ~2dField();
        //-----------------------------------------

        int solve_field(std::vector<double> re, std::vector<double> im);
        void print_field();
};

#endif
