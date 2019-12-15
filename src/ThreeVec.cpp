#include "ThreeVec.h"

/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/

/**
 * @brief Constructor for ThreeVec object
 *
 */
ThreeVec::ThreeVec() : ThreeVec(0.0, 0.0, 0.0)
{
}

/**
 * @brief Cartesian constructor for ThreeVec object
 *
 * @param x x coordinate
 * @param y y coordinate
 * @param z z coordinate
 */
ThreeVec::ThreeVec(double x, double y, double z)
{
    coord_[X_IDX] = x;
    coord_[Y_IDX] = y;
    coord_[Z_IDX] = z;
}

/**
 * @brief Destructor for ThreeVec object
 *
 */
ThreeVec::~ThreeVec()
{
}
//-----------------------------------------


/**********************************************************
CLASS METHODS
***********************************************************/

// Operations
/**
 * @brief Alternative modifier method for ith coordinate -> ADD
 *
 * @param i Index into the vector
 * @param value Value to add to the ith element
 */
void ThreeVec::inc(int i, double value)
{
    coord_[i] += value;
}

/**
 * @brief Square the ThreeVec
 *
 * @return double The sum of the squares of all components
 */
double ThreeVec::square()
{
    double answer = 0.0;
    for (uint i = 0; i < MAX_DIM; ++i)
    {
        answer += coord_[i] * coord_[i];
    }
    return answer;
}

/**
 * @brief Calculate the magnitude of the ThreeVec
 *
 * @return double Magnitude of the ThreeVec
 */
double ThreeVec::mag()
{
    return sqrt(square());
}

/**
 * @brief Perform element-wise multiplication
 *
 * @param vec Vector to multiply element-wise with
 * @return ThreeVec Resultant ThreeVec
 */
ThreeVec ThreeVec::element_multiply(const ThreeVec& vec)
{
    ThreeVec ans(coord_[X_IDX] * vec.get_x(),
                 coord_[Y_IDX] * vec.get_y(),
                 coord_[Z_IDX] * vec.get_z());
    return ans;
}


// Print Functions
/**
 * @brief Prints all components of the vector
 *
 */
void ThreeVec::print() const
{
    for (uint i = 0; i < MAX_DIM; ++i)
    {
        std::cout << coord_[i] << '\t';
    }
    std::cout << std::endl;
}

/**
 * @brief Prints a single component of the vector
 *
 * @param i The index of the component to print
 */
void ThreeVec::print_comp(uint i) const
{
    std::cout << coord_[i] << std::endl;
}
//-----------------------------------------
