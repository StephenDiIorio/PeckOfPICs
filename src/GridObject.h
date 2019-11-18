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
        GridObject(uint nx, uint ny);
        GridObject(uint nx, uint ny, std::function<void(GridObject &, uint, uint)> init_fcn);
        GridObject(uint nx, uint ny, std::vector<double> data); // a 'copy' constructor
        ~GridObject();
        //-----------------------------------------

        // inline void set_grid_data(uint index_x, uint index_y, double val);
        // inline void add_to_grid_data(uint index_x, uint index_y, double val);
        // inline double get_grid_data(uint index_x, uint index_y) const;
        inline void add_to_grid_data(uint index_x, uint index_y, double val)
        {
            gridded_data.at(index_x * ny + index_y) += val;
        }

        inline void set_grid_data(uint index_x, uint index_y, double val)
        {
            gridded_data.at(index_x * ny + index_y) = val;
        }

        inline double get_grid_data(uint index_x, uint index_y) const
        {
            return gridded_data.at(index_x * ny + index_y);
        }

        inline void print_grid_data()
        {
        for (auto &f : this->gridded_data )
            {
                std::cout << f << '\t';
            }
            std::cout << std::endl;
        }
};
#endif
