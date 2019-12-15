#ifndef DATA_STORAGE_2D_H
#define DATA_STORAGE_2D_H

#include <iostream>
// #include <functional>
#include <math.h>

#include "DataStorage.h"

class DataStorage_2D : public DataStorage
{
    private:
        const std::size_t ndims = 2;

        std::size_t Nx;
        std::size_t Ny;

        /**********************************************************
        PRIVATE CLASS METHODS
        ***********************************************************/
        bool same_size(const DataStorage_2D& other_storage) const;

        // void init_data_storage(std::function<void(DataStorage_2D&,
        //                                           std::size_t, std::size_t)> init_fcn);
        //-----------------------------------------

    public:
        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        DataStorage_2D(); // Note: need an empty constructor to allow DataStorage_2D to be a member
        DataStorage_2D(std::size_t Nx, std::size_t Ny);
        DataStorage_2D(std::size_t Nx, std::size_t Ny, double val);
        // DataStorage_2D(std::size_t Nx, std::size_t Ny,
        //                std::function<void(DataStorage_2D&,
        //                                   std::size_t, std::size_t)> init_fcn);
        DataStorage_2D(std::size_t Nx, std::size_t Ny,
                       std::vector<double> data);       // a 'copy' constructor
        DataStorage_2D(const DataStorage_2D& copy_obj); // a copy constructor
        ~DataStorage_2D();
        //-----------------------------------------


        /**********************************************************
        OPERATOR FUNCTIONS
        ***********************************************************/
        DataStorage_2D &operator=(const DataStorage_2D &to_copy);

        // Indexing operator
        /**
         * @brief Returns a reference to the element at specified location
         *        (x1, x2). No bounds checking is performed.
         *
         * @param x1 Position of the element in first dimension to return
         * @param x2 Position of the element in second dimension to return
         * @return double& Reference to the requested element
         */
        inline double &operator()(const std::size_t x1, const std::size_t x2)
        {
            return this->data[x1 * this->Ny + x2];
        }

        /**
         * @brief Returns a reference to the element at specified location
         *        (x1, x2). No bounds checking is performed.
         *
         * @param x1 Position of the element in first dimension to return
         * @param x2 Position of the element in second dimension to return
         * @return double& Reference to the requested element
         */
        inline const double& operator()(const std::size_t x1, const std::size_t x2) const
        {
            return this->data[x1 * this->Ny + x2];
        }


        // Addition with other DataStorage and with type double
        /**
         * @brief Element-wise add two DataStorage objects
         *
         * @param d1 DataStorage left of the operator
         * @param d2 DataStorage right of the operator
         * @return DataStorage_2D Resultant DataStorage
         */
        friend DataStorage_2D operator+(DataStorage_2D d1, const DataStorage_2D& d2)
        {
            d1 += d2;
            return d1;
        }

        /**
         * @brief Element-wise addition with DataStorage and a scalar
         *
         * @param d DataStorage left of the operator
         * @param val Scalar right of the operator
         * @return DataStorage_2D Resultant DataStorage
         */
        friend DataStorage_2D operator+(DataStorage_2D d, const double val)
        {
            d += val;
            return d;
        }


        // Subtraction with other DataStorage and with type double
        /**
         * @brief Element-wise subtraction two DataStorage objects
         *
         * @param d1 DataStorage left of the operator
         * @param d2 DataStorage right of the operator
         * @return DataStorage_2D Resultant DataStorage
         */
        friend DataStorage_2D operator-(DataStorage_2D d1, const DataStorage_2D& d2)
        {
            d1 -= d2;
            return d1;
        }

        /**
         * @brief Element-wise subtraction with DataStorage and a scalar
         *
         * @param d DataStorage left of the operator
         * @param val Scalar right of the operator
         * @return DataStorage_2D Resultant DataStorage
         */
        friend DataStorage_2D operator-(DataStorage_2D d, const double val)
        {
            d -= val;
            return d;
        }


        // Multiplication with other DataStorage and with type double
        /**
         * @brief Element-wise multiplication two DataStorage objects
         *
         * @param d1 DataStorage left of the operator
         * @param d2 DataStorage right of the operator
         * @return DataStorage_2D Resultant DataStorage
         */
        friend DataStorage_2D operator*(DataStorage_2D d1, const DataStorage_2D& d2)
        {
            d1 *= d2;
            return d1;
        }

        /**
         * @brief Element-wise multiplication with DataStorage and a scalar
         *
         * @param d DataStorage left of the operator
         * @param val Scalar right of the operator
         * @return DataStorage_2D Resultant DataStorage
         */
        friend DataStorage_2D operator*(DataStorage_2D d, const double val)
        {
            d *= val;
            return d;
        }


        // Division with other DataStorage and with type double
        /**
         * @brief Element-wise division two DataStorage objects
         *
         * @param d1 DataStorage left of the operator
         * @param d2 DataStorage right of the operator
         * @return DataStorage_2D Resultant DataStorage
         */
        friend DataStorage_2D operator/(DataStorage_2D d1, const DataStorage_2D& d2)
        {
            d1 /= d2;
            return d1;
        }

        /**
         * @brief Element-wise division with DataStorage and a scalar
         *
         * @param d DataStorage left of the operator
         * @param val Scalar right of the operator
         * @return DataStorage_2D Resultant DataStorage
         */
        friend DataStorage_2D operator/(DataStorage_2D d, const double val)
        {
            d /= val;
            return d;
        }


        // Addition/Assignment with other DataStorage and double
        /**
         * @brief Element-wise addition and assignment for DataStorage objects
         *
         * @param other_data DataStorage right of the operator
         * @return DataStorage_2D& This object after operation
         */
        inline DataStorage_2D& operator+=(const DataStorage_2D& other_data)
        {
            if (!this->same_size(other_data))
            {
                throw std::runtime_error(same_size_err);
            }

            for (std::size_t i = 0; i < this->size; ++i)
            {
                this->data[i] += other_data.data[i];
            }

            return *this;
        }

        /**
         * @brief Element-wise addition and assignment for DataStorage and
         *        scalar
         *
         * @param val Scalar right of the operator
         * @return DataStorage& This object after operation
         */
        inline DataStorage_2D& operator+=(const double val)
        {
            for (std::size_t i = 0; i < this->size; ++i)
            {
                this->data[i] += val;
            }

            return *this;
        }


        // Subtraction/Assignment with other DataStorage and double
        /**
         * @brief Element-wise subtraction and assignment for DataStorage
         *        objects
         *
         * @param other_data DataStorage right of the operator
         * @return DataStorage_2D& This object after operation
         */
        inline DataStorage_2D& operator-=(const DataStorage_2D& other_data)
        {
            if (!this->same_size(other_data))
            {
                throw std::runtime_error(same_size_err);
            }

            for (std::size_t i = 0; i < this->size; ++i)
            {
                this->data[i] -= other_data.data[i];
            }

            return *this;
        }

        /**
         * @brief Element-wise subtraction and assignment for DataStorage and
         *        scalar
         *
         * @param val Scalar right of the operator
         * @return DataStorage_2D& This object after operation
         */
        inline DataStorage_2D& operator-=(const double val)
        {
            for (std::size_t i = 0; i < this->size; ++i)
            {
                this->data[i] -= val;
            }

            return *this;
        }


        // Multiplication/Assignment with other DataStorage and double
        /**
         * @brief Element-wise multiplication and assignment for DataStorage
         *        objects
         *
         * @param other_data DataStorage right of the operator
         * @return DataStorage_2D& This object after operation
         */
        inline DataStorage_2D& operator*=(const DataStorage_2D& other_data)
        {
            if (!this->same_size(other_data))
            {
                throw std::runtime_error(same_size_err);
            }

            for (std::size_t i = 0; i < this->size; ++i)
            {
                this->data[i] *= other_data.data[i];
            }

            return *this;
        }

        /**
         * @brief Element-wise multiplication and assignment for DataStorage and
         *        scalar
         *
         * @param val Scalar right of the operator
         * @return DataStorage_2D& This object after operation
         */
        inline DataStorage_2D& operator*=(const double val)
        {
            for (std::size_t i = 0; i < this->size; ++i)
            {
                this->data[i] *= val;
            }

            return *this;
        }


        // Division/Assignment with other DataStorage and double
        /**
         * @brief Element-wise division and assignment for DataStorage objects
         *
         * @param other_data DataStorage right of the operator
         * @return DataStorage_2D& This object after operation
         */
        inline DataStorage_2D& operator/=(const DataStorage_2D& other_data)
        {
            if (!this->same_size(other_data))
            {
                throw std::runtime_error(same_size_err);
            }

            for (std::size_t i = 0; i < this->size; ++i)
            {
                this->data[i] /= other_data.data[i];
            }

            return *this;
        }

        /**
         * @brief Element-wise division and assignment for DataStorage and
         *        scalar
         *
         * @param val Scalar right of the operator
         * @return DataStorage_2D& This object after operation
         */
        inline DataStorage_2D& operator/=(const double val)
        {
            for (std::size_t i = 0; i < this->size; ++i)
            {
                this->data[i] /= val;
            }

            return *this;
        }
        //-----------------------------------------


        /**********************************************************
        CLASS METHODS
        ***********************************************************/

        double& at(const std::size_t x1, const std::size_t x2);
        const double& at(const std::size_t idx, const std::size_t x2) const;

        /**
         * @brief Get the number of Nx points in DataStorage object
         *
         * @return std::size_t Number of Nx points in DataStorage
         */
        inline std::size_t get_Nx() const
        {
            return this->Nx;
        }

        /**
         * @brief Get the number of Ny points in DataStorage object
         *
         * @return std::size_t Number of Ny points in DataStorage
         */
        inline std::size_t get_Ny() const
        {
            return this->Ny;
        }

        /**
         * @brief Get the dimensionality of the object
         *
         * @return std::size_t The dimensionality of the object
         */
        inline std::size_t get_ndims() const
        {
            return this->ndims;
        }

        std::size_t get_Ni_size(std::size_t i) const;

        bool equals(const DataStorage_2D& other_grid, const double TOL) const;

        //Printing Functions
        void print() const;
        void print_comp(const std::size_t xi, const std::size_t yi) const;
        std::ostream &print(std::ostream &s) const;
        //-----------------------------------------
};

#endif
