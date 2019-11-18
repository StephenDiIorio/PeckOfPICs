#include "GridObject.h"


/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/
GridObject::GridObject()
{
}

GridObject::GridObject(uint nx, uint ny)
{
    this->nx = nx;
    this->ny = ny;

    this->gridded_data = std::vector<double>(nx * ny, 0.0);
}

GridObject::GridObject(uint nx, uint ny, std::function<void(GridObject &, uint, uint)> init_fcn)
{
    this->nx = nx;
    this->ny = ny;

    init_grid_obj(init_fcn);
}

GridObject::GridObject(uint nx, uint ny, std::vector<double> data) // a 'copy' constructor
{
    this->nx = nx;
    this->ny = ny;

    this->gridded_data = data;
}

GridObject::~GridObject()
{
}
//-----------------------------------------


// inline void GridObject::add_to_grid_data(uint index_x, uint index_y, double val)
// {
//     gridded_data.at(index_x * ny + index_y) += val;
// }

// inline void GridObject::set_grid_data(uint index_x, uint index_y, double val)
// {
//     gridded_data.at(index_x * ny + index_y) = val;
// }

// inline double GridObject::get_grid_data(uint index_x, uint index_y) const
// {
//     return gridded_data.at(index_x * ny + index_y);
// }




/**********************************************************
PRIVATE FUNCTIONS
***********************************************************/
void GridObject::init_grid_obj(std::function<void(GridObject &, uint, uint)> init_fcn)
{
    init_fcn(*this, this->nx, this->ny);
}
