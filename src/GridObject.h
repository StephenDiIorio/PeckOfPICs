#ifndef GRID_H
#define GRID_H

// #include <stdio.h>
#include <iostream>
#include <vector>
#include <functional>
typedef unsigned int uint;

class GridObject 
{
    private:
        std::vector<double> gridded_data;
        int nx, ny;

        void init_grid_obj(std::function<void(GridObject &, uint, uint)> init_fcn);

    public:
        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        GridObject(); //TODO: see if this can be removed
        GridObject(uint nx, uint ny, std::function<void(GridObject &, uint, uint)> init_fcn);
        GridObject(uint nx, uint ny, std::vector<double> data); // a 'copy' constructor
        ~GridObject();
        //-----------------------------------------

        inline double get_grid_data(uint index_x, unsigned int index_y);
        void print_grid_data();
};
#endif
