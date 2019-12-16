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
 * @brief Constructor for GridObject object - sets to an Nx by Ny grid of 0s
 *
 * @param Nx Number of x values
 * @param Ny Number of y values
 */
GridObject::GridObject(std::size_t Nx, std::size_t Ny) : GridObject(Nx, Ny, 0.0)
{
}

/**
 * @brief Construct a new GridObject object - sets to an Nx by Ny grid of val
 *
 * @param Nx Number of x values
 * @param Ny Number of y values
 * @param val Value to initialize grid to
 */
GridObject::GridObject(std::size_t Nx, std::size_t Ny, double val)
{
    this->Nx = Nx;
    this->Ny = Ny;

    this->gridded_data = DataStorage_2D(Nx, Ny, val);
}

/**
 * @brief Constructor for GridObject object
 *
 * @param Nx Number of x values
 * @param Ny Number of y values
 * @param init_fcn User provided function which initializes the data in the grid
 *                 to the user's specification
 */
GridObject::GridObject(std::size_t Nx, std::size_t Ny, std::function<void(GridObject &, std::size_t, std::size_t)> init_fcn)
{
    this->Nx = Nx;
    this->Ny = Ny;

    init_grid_obj(init_fcn);
}

/**
 * @brief Constructor for GridObject object
 *
 * @param Nx Number of x values
 * @param Ny Number of y values
 * @param data A vector object to copy the data from into the current grid
 */
GridObject::GridObject(std::size_t Nx, std::size_t Ny, std::vector<double> data) // a 'copy' constructor
{
    if ((Nx * Ny) != data.size())
    {
        throw std::runtime_error("Specified dimensions of grid do not match number of supplied data points.");
    }

    this->Nx = Nx;
    this->Ny = Ny;

    this->gridded_data = DataStorage_2D(Nx, Ny, data);
}

/**
 * @brief Constructor for GridObject object
 *
 * @param copy_obj
 */
GridObject::GridObject(GridObject const &copy_obj)
{
    this->Nx = copy_obj.Nx;
    this->Ny = copy_obj.Ny;
    this->gridded_data = copy_obj.gridded_data;
}

/**
 * @brief Destructor for GridObject object
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
void GridObject::print() const
{
    this->gridded_data.print();
}

/**
 * @brief Print a single entry of grid at given coordinates
 *
 * @param xi X index of grid value to print
 * @param yj Y index of grid value to print
 */
void GridObject::print_comp(std::size_t xi, std::size_t yj) const
{
    this->gridded_data.print_comp(xi, yj);
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
bool GridObject::equals(const GridObject &other_obj, const double TOL) const
{
    if (other_obj.Nx != this->Nx || other_obj.Ny != this->Ny) //TODO: this check necessary?
    {
        return false;
    }
    else
    {
        return gridded_data.equals(other_obj.gridded_data, TOL);
    }
}

/**
 * @brief Zeros out all of the data stored in the object
 *
 */
void GridObject::zero()
{
    this->gridded_data.zero();
}
//-----------------------------------------


/**********************************************************
PRIVATE FUNCTIONS
***********************************************************/
void GridObject::init_grid_obj(std::function<void(GridObject &, std::size_t, std::size_t)> init_fcn)
{
    init_fcn(*this, this->Nx, this->Ny);
}
//-----------------------------------------
