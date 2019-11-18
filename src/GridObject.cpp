#include "GridObject.h"

/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/

/**
 * @brief Constructor for Grid Object:: Grid Object object
 *
 */
GridObject::GridObject()
{
}

/**
 * @brief Constructor for Grid Object:: Grid Object object
 *
 * @param nx Number of x values
 * @param ny Number of y values
 */
GridObject::GridObject(uint nx, uint ny)
{
    this->nx = nx;
    this->ny = ny;

    this->gridded_data = std::vector<double>(nx * ny, 0.0);
}

/**
 * @brief Constructor for Grid Object:: Grid Object object
 *
 * @param nx Number of x values
 * @param ny Number of y values
 * @param init_fcn User provided function which initializes the data in the grid
 *                 to the user's specification
 */
GridObject::GridObject(uint nx, uint ny, std::function<void(GridObject &, uint, uint)> init_fcn)
{
    this->nx = nx;
    this->ny = ny;

    init_grid_obj(init_fcn);
}

/**
 * @brief Constructor for Grid Object:: Grid Object object
 *
 * @param nx Number of x values
 * @param ny Number of y values
 * @param data A vector object to copy the data from into the current grid
 */
GridObject::GridObject(uint nx, uint ny, std::vector<double> data) // a 'copy' constructor
{
    this->nx = nx;
    this->ny = ny;

    this->gridded_data = std::vector<double>(data);
}

/**
 * @brief Destructor for Grid Object:: Grid Object object
 *
 */
GridObject::~GridObject()
{
}
//-----------------------------------------


/**
 * @brief Print the values stored within the grid object
 *
 */
void GridObject::print_grid_data()
{
    for (auto &d : this->gridded_data )
    {
        std::cout << d << '\t';
    }
    std::cout << std::endl;
}


/**********************************************************
PRIVATE FUNCTIONS
***********************************************************/
void GridObject::init_grid_obj(std::function<void(GridObject &, uint, uint)> init_fcn)
{
    init_fcn(*this, this->nx, this->ny);
}
