#include "DataStorage.h"

/**********************************************************
CLASS METHODS
***********************************************************/

/**
 * @brief Returns a reference to the element at specified location idx, with
 *        bounds checking. If idx is not within the range of the container, an
 *        exception of type std::out_of_range is thrown.
 *
 * @param idx Position of the element to return.
 * @return double& Reference to the requested element.
 */
double& DataStorage::at(const size_t idx)
{
    return this->data.at(idx);
}

/**
 * @brief Returns a reference to the element at specified location idx, with
 *        bounds checking. If idx is not within the range of the container, an
 *        exception of type std::out_of_range is thrown.
 *
 * @param idx Position of the element to return.
 * @return double& Reference to the requested element.
 */
const double& DataStorage::at(const std::size_t idx) const
{
    return this->data.at(idx);
}
//-----------------------------------------
