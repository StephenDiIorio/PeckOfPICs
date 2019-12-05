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
 * @brief Construct a new Grid Object object
 *
 * @param Nx
 * @param Ny
 * @param val
 */
GridObject::GridObject(uint Nx, uint Ny, double val)
{
    this->Nx = Nx;
    this->Ny = Ny;

    this->gridded_data = std::vector<double>(Nx * Ny, val);
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

GridObject::GridObject(GridObject const &copy_obj)
{
    this->Nx = copy_obj.Nx;
    this->Ny = copy_obj.Ny;
    this->gridded_data = copy_obj.gridded_data;
}

/**
 * @brief Destructor for Grid Object object
 *
*/
GridObject::~GridObject()
{
}
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
    for (int xi = 0; xi < this->Nx; ++xi)
    {
        for (int yj = 0; yj < this->Ny; ++yj)
        {
            std::cout << this->get_grid_data(xi,yj) << '\t';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Compares whether or not two GridObjects are equivalent
 *
 * @param other_obj Other GridObject to compare against
 * @param TOL Tolerance value to compare elements within
 * @return true If the two grids are of the same size and contain the same
 *              elements, within the provided tolerance
 * @return false If either of the two grids has a different size and if any of
 *               the values differ above the provided tolerance
 */
bool GridObject::compare_with(GridObject const &other_obj, double const TOL)
{
    if (other_obj.Nx != this->Nx || other_obj.Ny != this->Ny)
    {
        return false;
    }
    else
    {
        for (int xi = 0; xi < this->Nx; ++xi)
        {
            for (int yj = 0; yj < this->Ny; ++yj)
            {
                if (fabs(this->get_grid_data(xi, yj) -
                         other_obj.get_grid_data(xi,yj)) >= TOL)
                {
                    return false;
                }
            }
        }
        return true;
    }
}
//-----------------------------------------


/**********************************************************
PRIVATE FUNCTIONS
***********************************************************/
void GridObject::init_grid_obj(std::function<void(GridObject &, uint, uint)> init_fcn)
{
    init_fcn(*this, this->Nx, this->Ny);
}
