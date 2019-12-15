#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include <functional>
#include <math.h>

#include "DataStorage_2D.h"

#define MODULO(a, b) (int)((a % b) >= 0 ? (a % b) : (a % b) + b)

/**
 * @brief A 2D Grid that can be used to store data
 *
 */
class GridObject
{
    private:
        /**********************************************************
        PRIVATE CLASS METHODS
        ***********************************************************/
        void init_grid_obj(std::function<void(GridObject &, uint, uint)> init_fcn);
        //-----------------------------------------

    public:
        DataStorage_2D gridded_data;
        int Nx, Ny;

        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        GridObject(); // Note: need an empty constructor to allow GridObject to be a member
        GridObject(uint Nx, uint Ny);
        GridObject(uint Nx, uint Ny, double val);
        GridObject(uint Nx, uint Ny,
                   std::function<void(GridObject &, uint, uint)> init_fcn);
        GridObject(uint Nx, uint Ny,
                   std::vector<double> data);   // a 'copy' constructor
        GridObject(GridObject const &copy_obj); // a copy constructor
        ~GridObject();
        //-----------------------------------------


        /**********************************************************
        ITERATOR FUNCTIONS
        ***********************************************************/
        using iterator = std::vector<double>::iterator;
        using const_iterator = std::vector<double>::const_iterator;

        inline iterator begin() noexcept
        {
            return gridded_data.begin();
        }
        inline const_iterator cbegin() const noexcept
        {
            return gridded_data.cbegin();
        }
        inline iterator end() noexcept
        {
            return gridded_data.end();
        }
        inline const_iterator cend() const noexcept
        {
            return gridded_data.cend();
        }
        //-----------------------------------------


        /**********************************************************
        OPERATOR FUNCTIONS
        ***********************************************************/
        /**
         * @brief Overload the += operator for element-wise addition between
         *        GridObject objects
         *
         */
        inline GridObject operator+=(const GridObject& grid)
        {
            this->gridded_data += grid.gridded_data;
            return *(this);
        }

        /**
         * @brief Overload the -= operator for element-wise subtraction between
         *        GridObject objects
         *
         */
        inline GridObject operator-=(const GridObject& grid)
        {
            this->gridded_data -= grid.gridded_data;
            return *(this);
        }
        //-----------------------------------------


        /**********************************************************
        CLASS METHODS
        ***********************************************************/
        /**
         * @brief Add the provided value to the current value stored in the
         *        grid at a given index.
         *
         * @param index_x x index of grid
         * @param index_y y index of grid
         * @param val Number to sum with value at given indices
         */
        inline void comp_add_to(uint index_x, uint index_y, double val)
        {
            index_x = MODULO(index_x, this->Nx);
            index_y = MODULO(index_y, this->Ny);
            gridded_data.at(index_x, index_y) += val;
        }

        /**
         * @brief Multiply the provided value to the current value stored in the
         *        grid at a given index.
         *
         * @param index_x x index of grid
         * @param index_y y index of grid
         * @param val Number to multiply with value at given indices
         */
        inline void comp_multiply_by(uint index_x, uint index_y, double val)
        {
            index_x = MODULO(index_x, this->Nx);
            index_y = MODULO(index_y, this->Ny);
            gridded_data.at(index_x, index_y) *= val;
        }


        /**
         * @brief Set the grid data object
         *
         * @param index_x x index of grid
         * @param index_y y index of grid
         * @param val Number to set in grid at given indices
         */
        inline void set_comp(uint index_x, uint index_y, double val)
        {
            index_x = MODULO(index_x, this->Nx);
            index_y = MODULO(index_y, this->Ny);
            gridded_data.at(index_x, index_y) = val;
        }

        /**
         * @brief Get the grid data object
         *
         * @param index_x x index of grid
         * @param index_y y index of grid
         * @return double Value stored in grid at given indices
         */
        inline double get_comp(uint index_x, uint index_y) const
        {
            index_x = MODULO(index_x, this->Nx);
            index_y = MODULO(index_y, this->Ny);
            return gridded_data.at(index_x, index_y);
        }

        /**
         * @brief Get the data object
         *
         * @return const DataStorage_2D&
         */
        inline const DataStorage_2D& get_data() const
        {
            return this->gridded_data;
        }

        /**
         * @brief Get the number of Nx points in DataStorage object
         *
         * @return std::size_t Number of Nx points in DataStorage
         */
        inline double get_Nx() const
        {
            return gridded_data.get_Nx();
        }

        /**
         * @brief Get the number of Ny points in DataStorage object
         *
         * @return std::size_t Number of Ny points in DataStorage
         */
        inline double get_Ny() const
        {
            return gridded_data.get_Ny();
        }

        /**
         * @brief Get the number of elements in the ith component of the data
         *        structure
         *
         * @param i The ith component to retrieve
         * @return std::size_t The number of elements in the ith component
         */
        inline double get_Ni_size(std::size_t i) const
        {
            return gridded_data.get_Ni_size(i);
        }


        void print() const;
        void print_comp(uint xi, uint yj) const;

        bool equals(const GridObject &other_obj, const double TOL) const;

        void zero();
        //-----------------------------------------
};

#endif
