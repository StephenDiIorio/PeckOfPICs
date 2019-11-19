#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include <functional>

#define MODULO(a, b) (int)((a % b) >= 0 ? (a % b) : (a % b) + b)

typedef unsigned int uint;

class GridObject
{
    private:
        std::vector<double> gridded_data;
        int Nx, Ny;

        void init_grid_obj(std::function<void(GridObject &, uint, uint)> init_fcn);

    public:
        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        // GridObject(); //TODO: see if this can be removed
        GridObject(uint Nx, uint Ny);
        GridObject(uint Nx, uint Ny,
                   std::function<void(GridObject &, uint, uint)> init_fcn);
        GridObject(uint Nx, uint Ny,
                   std::vector<double> data); // a 'copy' constructor
        ~GridObject();
        //-----------------------------------------


        /**********************************************************
        ITERATOR FUNCTIONS
        ***********************************************************/
        typedef typename std::vector<double>::iterator iterator;
        typedef typename std::vector<double>::const_iterator const_iterator;

        inline iterator begin() noexcept
        {
            return gridded_data.begin();
        }
        inline GridObject::const_iterator cbegin() const noexcept
        {
            return gridded_data.cbegin();
        }
        inline GridObject::iterator end() noexcept
        {
            return gridded_data.end();
        }
        inline GridObject::const_iterator cend() const noexcept
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
            gridded_data.at(index_x * this->Ny + index_y) += val;
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
            return gridded_data.at(index_x * this->Ny + index_y);
        }


        /**
         * @brief Get the left neighbor of current index
         *
         * @param index_x x index of grid
         * @param index_y y index of grid
         * @return double Value stored in grid at left neighbor of given
         *                indices
         */
        inline double get_left_neighbor(uint index_x, uint index_y) const
        {
            uint left_x = MODULO(index_x-1, this->Nx);
            return gridded_data.at(left_x * this->Ny + index_y);
        }

        /**
         * @brief Get the right neighbor of current index
         *
         * @param index_x x index of grid
         * @param index_y y index of grid
         * @return double Value stored in grid at right neighbor of given
         *                indices
         */
        inline double get_right_neighbor(uint index_x, uint index_y) const
        {
            uint right_x = MODULO(index_x+1, this->Nx);
            return gridded_data.at(right_x * this->Ny + index_y);
        }

        /**
         * @brief Get the upper neighbor of current index
         *
         * @param index_x x index of grid
         * @param index_y y index of grid
         * @return double Value stored in grid at upper neighbor of given
         *                indices
         */
        inline double get_upper_neighbor(uint index_x, uint index_y) const
        {
            uint upper_y = MODULO(index_y+1, this->Ny);
            return gridded_data.at(index_x * this->Ny + upper_y);
        }

        /**
         * @brief Get the lower neighbor of current index
         *
         * @param index_x x index of grid
         * @param index_y y index of grid
         * @return double Value stored in grid at lower neighbor of given
         *                indices
         */
        inline double get_lower_neighbor(uint index_x, uint index_y) const
        {
            uint lower_y = MODULO(index_y-1, this->Ny);
            return gridded_data.at(index_x * this->Ny + lower_y);
        }


        void print_grid_data();
        //-----------------------------------------
};

#endif
