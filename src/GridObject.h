#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include <functional>
#include <math.h>

#define MODULO(a, b) (int)((a % b) >= 0 ? (a % b) : (a % b) + b)

typedef unsigned int uint;

class GridObject
{
    private:
        void init_grid_obj(std::function<void(GridObject &, uint, uint)> init_fcn);

    public:
        std::vector<double> gridded_data;
        int Nx, Ny;

        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        GridObject(); //TODO: see if this can be removed
        // note: need an empty constructor to allow GridObject to be a member
        GridObject(uint Nx, uint Ny);
        GridObject(uint Nx, uint Ny, double val);
        GridObject(uint Nx, uint Ny,
                   std::function<void(GridObject &, uint, uint)> init_fcn);
        GridObject(uint Nx, uint Ny,
                   std::vector<double> data); // a 'copy' constructor
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
        inline GridObject operator+=(GridObject grid)
        {
            for (uint i = 0; i < this->Nx * this->Ny; ++i)
            {
                this->gridded_data.at(i) += grid.gridded_data.at(i);
            }

            return *(this);
        }

        /**
         * @brief Overload the -= operator for element-wise subtraction between
         *        GridObject objects
         *
         */
        inline GridObject operator-=(GridObject grid)
        {
            for (uint i = 0; i < this->Nx * this->Ny; ++i)
            {
                this->gridded_data.at(i) -= grid.gridded_data.at(i);
            }

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
        inline void add_to_grid_data(uint index_x, uint index_y, double val)
        {
            index_x = MODULO(index_x, this->Nx);
            index_y = MODULO(index_y, this->Ny);
            gridded_data.at(index_x * this->Ny + index_y) += val;
        }

        inline void multiply_grid_data_by(uint index_x, uint index_y, double val)
        {
            index_x = MODULO(index_x, this->Nx);
            index_y = MODULO(index_y, this->Ny);
            gridded_data.at(index_x * this->Ny + index_y) *= val;
        }


        /**
         * @brief Set the grid data object
         *
         * @param index_x x index of grid
         * @param index_y y index of grid
         * @param val Number to set in grid at given indices
         */
        inline void set_grid_data(uint index_x, uint index_y, double val)
        {
            index_x = MODULO(index_x, this->Nx);
            index_y = MODULO(index_y, this->Ny);
            gridded_data.at(index_x * this->Ny + index_y) = val;
        }

        /**
         * @brief Get the grid data object
         *
         * @param index_x x index of grid
         * @param index_y y index of grid
         * @return double Value stored in grid at given indices
         */
        inline double get_grid_data(uint index_x, uint index_y) const
        {
            index_x = MODULO(index_x, this->Nx);
            index_y = MODULO(index_y, this->Ny);
            return gridded_data.at(index_x * this->Ny + index_y);
        }


        void print_grid_data();

        bool compare_with(GridObject const &other_obj, double const TOL);
        //-----------------------------------------
};

#endif
