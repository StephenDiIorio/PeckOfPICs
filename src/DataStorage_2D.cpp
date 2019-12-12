#include "DataStorage_2D.h"

/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/

/**
 * @brief Constructor for DataStorage_2D object
 *
 */
DataStorage_2D::DataStorage_2D()
{
}

/**
 * @brief Constructor for DataStorage_2D object - sets to a grid of Nx by Ny 0s
 *
 * @param Nx Number of x values
 * @param Ny Number of y values
 */
DataStorage_2D::DataStorage_2D(std::size_t Nx, std::size_t Ny) : DataStorage_2D(Nx, Ny, 0.0)
{
}

/**
 * @brief Construct a new DataStorage_2D object - sets to a grid of Nx by Ny
 *        vals
 *
 * @param Nx Number of x values
 * @param Ny Number of y values
 * @param val Value to initialize grid to
 */
DataStorage_2D::DataStorage_2D(std::size_t Nx, std::size_t Ny, double val)
{
    this->Nx = Nx;
    this->Ny = Ny;
    this->size = Nx * Ny;

    this->data = std::vector<double>(this->size, val);
}

// /**
//  * @brief Constructor for DataStorage_2D object - sets to grid defined by
//  *        init_fcn
//  *
//  * @param Nx Number of x values
//  * @param Ny Number of y values
//  * @param init_fcn User provided function which initializes the data in the grid
//  *                 to the user's specification
//  */
// DataStorage_2D::DataStorage_2D(std::size_t Nx, std::size_t Ny,
//                                std::function<void(DataStorage_2D&,
//                                                   std::size_t, std::size_t)> init_fcn)
// {
//     this->Nx = Nx;
//     this->Ny = Ny
//     this->size = Nx * Ny;

//     init_grid_obj(init_fcn);
// }

/**
 * @brief Constructor for DataStorage_2D object - stores the values in data in
 *        the DataStorage object
 *
 * @param Nx Number of x values
 * @param Ny Number of y values
 * @param data A vector object to copy the data from into the current grid
 */
DataStorage_2D::DataStorage_2D(std::size_t Nx, std::size_t Ny, std::vector<double> data) // a 'copy' constructor
{
    //TODO: check that nx and ny are the same individually as well?
    if ((Nx * Ny) != data.size())
    {
        throw std::runtime_error(same_size_err);
    }

    this->Nx = Nx;
    this->Ny = Ny;
    this->size = Nx * Ny;

    this->data = std::vector<double>(data); // TODO: copying this twice?
}

/**
 * @brief Constructor for DataStorage_2D object - copies from another
 *        DataStorage_2D
 *
 * @param copy_obj DataStorage_2D to copy from
 */
DataStorage_2D::DataStorage_2D(const DataStorage_2D& copy_obj)
{
    this->Nx = copy_obj.Nx;
    this->Ny = copy_obj.Ny;
    this->size = copy_obj.size;
    this->data = copy_obj.data;
}

/**
 * @brief Destructor for DataStorage_2D object
 *
*/
DataStorage_2D::~DataStorage_2D()
{
}
//-----------------------------------------


/**********************************************************
OPERATOR FUNCTIONS
***********************************************************/

/**
 * @brief Overload copy assignment operator
 *
 * @param to_copy Object to copy values from
 * @return DataStorage_1D& New opbject with copied values
 */
DataStorage_2D& DataStorage_2D::operator=(const DataStorage_2D& to_copy)
{
    Nx = to_copy.Nx;
    Ny = to_copy.Ny;
    size = to_copy.size;
    data = to_copy.data;
    return *this;
}
//-----------------------------------------


/**********************************************************
CLASS METHODS
***********************************************************/

/**
 * @brief Returns a reference to the element at specified location (x1,x2), with
 *        bounds checking. If index is not within the range of the container, an
 *        exception of type std::out_of_range is thrown.
 *
 * @param x1 Index to first dimension to access
 * @param x2 Index to second dimension to access
 * @return double& Reference to the requested element.
 */
double& DataStorage_2D::at(const std::size_t x1, const std::size_t x2)
{
    return this->DataStorage::at(x1 * this->Ny + x2);
}

/**
 * @brief Returns a reference to the element at specified location (x1,x2), with
 *        bounds checking. If index is not within the range of the container, an
 *        exception of type std::out_of_range is thrown.
 *
 * @param x1 Index to first dimension to access
 * @param x2 Index to second dimension to access
 * @return double& Reference to the requested element.
 */
const double& DataStorage_2D::at(const std::size_t x1, const std::size_t x2) const
{
    return this->DataStorage::at(x1 * this->Ny + x2);
}

/**
 * @brief Get the number of elements in the ith component of the data structure
 *
 * @param i The ith component to retrieve
 * @return std::size_t The number of elements in the ith component
 */
std::size_t DataStorage_2D::get_Ni_size(std::size_t i) const
{
    switch (i)
    {
        case x1_accessor:
            return this->get_Nx();
            break;

        case x2_accessor:
            return this->get_Ny();
            break;

        default:
            throw std::runtime_error(no_dimension_err); break;
    }
}

/**
 * @brief Compares two DataStorage_2D objects to see if they are equivalent
 *
 * @param other_grid Other DataStorage_2D object to check equivalence against
 * @param TOL Tolerance value to compare elements within
 * @return true If the two objects are of the same size and contain the same
 *              elements, within the provided tolerance
 * @return false If either of the two objects has a different size or if any of
 *               the values differ above the provided tolerance
 */
bool DataStorage_2D::equals(const DataStorage_2D& other_grid, const double TOL) const
{
    if (!this->same_size(other_grid))
    {
        return false;
    }

    for (std::size_t i = 0; i < this->size; ++i)
    {
        if (fabs(this->data[i] - other_grid.data[i]) >= TOL)
        {
            return false;
        }
    }

    return true;
}

//Printing Functions
/**
 * @brief Prints the DataStorage_2D object to std::cout
 *
 */
void DataStorage_2D::print() const
{
    std::cout<< *this << std::endl;
}

/**
 * @brief Prints a single value stored at position (x1, x2) in DataStorage_2D
 *
 * @param x1 Index to first dimension of the item to print
 * @param x2 Index to second dimension of the item to print
 */
void DataStorage_2D::print_comp(const std::size_t x1, const std::size_t x2) const
{
    std::cout << this->at(x1, x2) << std::endl;
}

/**
 * @brief Prints the DataStorage_2D object to a IO stream
 *
 * @param s The IO stream to print the object to
 * @return std::ostream& The stream with the object printed to it
 */
std::ostream& DataStorage_2D::print(std::ostream &s) const
{
    for (std::size_t i = 0; i < this->Nx; ++i)
    {
        for (std::size_t j = 0; j < this->Ny; ++j)
        {
            s << this->at(i, j) << '\t';
        }
        s << '\n';
    }
    return s;
}
//-----------------------------------------


/**********************************************************
PRIVATE CLASS METHODS
***********************************************************/

/**
 * @brief Compares whether two DataStorage_2D objects have the same Nx
 *
 * @param other_storage The other object to compare against
 * @return true If the two objects have the same Nx
 * @return false If the two objects do not have the same Nx
 */
bool DataStorage_2D::same_size(const DataStorage_2D& other_storage) const
{
    return (this->Nx == other_storage.Nx && this->Ny == other_storage.Ny);
}
//-----------------------------------------
