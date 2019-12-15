#include "DataStorage_1D.h"

/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/

/**
 * @brief Constructor for DataStorage_1D object
 *
 */
DataStorage_1D::DataStorage_1D()
{
}

/**
 * @brief Constructor for DataStorage_1D object - sets to a grid of Nx 0s
 *
 * @param Nx Number of x values
 */
DataStorage_1D::DataStorage_1D(std::size_t Nx) : DataStorage_1D(Nx, 0.0)
{
}

/**
 * @brief Construct a new DataStorage_1D object - sets to a grid of Nx vals
 *
 * @param Nx Number of x values
 * @param val Value to initialize grid to
 */
DataStorage_1D::DataStorage_1D(std::size_t Nx, double val)
{
    this->Nx = Nx;
    this->size = Nx;

    this->data = std::vector<double>(this->size, val);
}

// /**
//  * @brief Constructor for DataStorage_1D object - sets to grid defined by
//  *        init_fcn
//  *
//  * @param Nx Number of x values
//  * @param init_fcn User provided function which initializes the data in the grid
//  *                 to the user's specification
//  */
// DataStorage_1D::DataStorage_1D(std::size_t Nx,
//                                std::function<void(DataStorage_1D&,
//                                                   std::size_t)> init_fcn)
// {
//     this->Nx = Nx;
//     this->size = Nx;

//     init_grid_obj(init_fcn);
// }

/**
 * @brief Constructor for DataStorage_1D object - stores the values in data in
 *        the DataStorage object
 *
 * @param Nx Number of x values
 * @param data A vector object to copy the data from into the current grid
 */
DataStorage_1D::DataStorage_1D(std::size_t Nx, std::vector<double> data) // a 'copy' constructor
{
    if (Nx != data.size())
    {
        throw std::runtime_error(same_size_err);
    }

    this->Nx = Nx;
    this->size = Nx;

    this->data = std::vector<double>(data);
}

/**
 * @brief Constructor for DataStorage_1D object - copies from another
 *        DataStorage_1D
 *
 * @param copy_obj DataStorage_1D to copy from
 */
DataStorage_1D::DataStorage_1D(const DataStorage_1D& copy_obj)
{
    this->Nx = copy_obj.Nx;
    this->size = copy_obj.size;
    this->data = copy_obj.data;
}

/**
 * @brief Destructor for DataStorage_1D object
 *
*/
DataStorage_1D::~DataStorage_1D()
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
DataStorage_1D& DataStorage_1D::operator=(const DataStorage_1D &to_copy)
{
    Nx = to_copy.Nx;
    size = to_copy.size;
    data = to_copy.data;
    return *this;
}
//-----------------------------------------


/**********************************************************
CLASS METHODS
***********************************************************/

/**
 * @brief Get the number of elements in the ith component of the data structure
 *
 * @param i The ith component to retrieve
 * @return std::size_t The number of elements in the ith component
 */
std::size_t DataStorage_1D::get_Ni_size(std::size_t i) const
{
    switch (i)
    {
        case x1_accessor:
            return this->get_Nx();
            break;

        default:
            throw std::runtime_error(no_dimension_err); break;
    }
}

/**
 * @brief Compares two DataStorage_1D objects to see if they are equivalent
 *
 * @param other_grid Other DataStorage_1D object to check equivalence against
 * @param TOL Tolerance value to compare elements within
 * @return true If the two objects are of the same size and contain the same
 *              elements, within the provided tolerance
 * @return false If either of the two objects has a different size or if any of
 *               the values differ above the provided tolerance
 */
bool DataStorage_1D::equals(const DataStorage_1D& other_grid, const double TOL)
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
 * @brief Prints the DataStorage_1D object to std::cout
 *
 */
void DataStorage_1D::print() const
{
    std::cout<< this << std::endl;
}

/**
 * @brief Prints a single value stored at position xi in DataStorage_1D
 *
 * @param x1 Index to the item to print
 */
void DataStorage_1D::print_comp(const std::size_t x1) const
{
    std::cout << this->at(x1) << std::endl;
}

/**
 * @brief Prints the DataStorage_1D object to a IO stream
 *
 * @param s The IO stream to print the object to
 * @return std::ostream& The stream with the object printed to it
 */
std::ostream& DataStorage_1D::print(std::ostream &s) const
{
    for (std::size_t i = 0; i < this->Nx; ++i)
    {
        s << this->at(i) << '\t';
    }
    return s;
}
//-----------------------------------------


/**********************************************************
PRIVATE CLASS METHODS
***********************************************************/

/**
 * @brief Compares whether two DataStorage_1D objects have the same Nx
 *
 * @param other_storage The other object to compare against
 * @return true If the two objects have the same Nx
 * @return false If the two objects do not have the same Nx
 */
bool DataStorage_1D::same_size(const DataStorage_1D& other_storage) const
{
    return (this->Nx == other_storage.Nx);
}
//-----------------------------------------
