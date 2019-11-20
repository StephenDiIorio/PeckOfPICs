#include "GridObject.h"

/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/

/**
 * @brief Constructor for Grid Object object
 *
 */
GridObject::GridObject()
{
}

/**
 * @brief Constructor for Grid Object object - sets to an nx x ny vector of 0s
 *
 * @param Nx Number of x values
 * @param Ny Number of y values
 */
GridObject::GridObject(uint Nx, uint Ny)
{
    this->Nx = Nx;
    this->Ny = Ny;

    this->gridded_data = std::vector<double>(Nx * Ny, 0.0);
}

/**
 * @brief Constructor for Grid Object object
 *
 * @param Nx Number of x values
 * @param Ny Number of y values
 * @param init_fcn User provided function which initializes the data in the grid
 *                 to the user's specification
 */
GridObject::GridObject(uint Nx, uint Ny, std::function<void(GridObject &, uint, uint)> init_fcn)
{
    this->Nx = Nx;
    this->Ny = Ny;

    init_grid_obj(init_fcn);
}

/**
 * @brief Constructor for Grid Object object
 *
 * @param Nx Number of x values
 * @param Ny Number of y values
 * @param data A vector object to copy the data from into the current grid
 */
GridObject::GridObject(uint Nx, uint Ny, std::vector<double> data) // a 'copy' constructor
{
    this->Nx = Nx;
    this->Ny = Ny;

    this->gridded_data = std::vector<double>(data);
}
// should there be a Destructor here?
/**
 * @brief Destructor for Grid Object object
}
*/
//-----------------------------------------


/**********************************************************
CLASS METHODS
***********************************************************/
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
//-----------------------------------------


/**********************************************************
PRIVATE FUNCTIONS
***********************************************************/
void GridObject::init_grid_obj(std::function<void(GridObject &, uint, uint)> init_fcn)
{
    init_fcn(*this, this->Nx, this->Ny);
}
