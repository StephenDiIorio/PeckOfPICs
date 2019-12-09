#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <iostream>
#include <vector>

/**
 * @brief DataStorage is the abstract interface for DataStorage objects used to
 *        store n-dimensional sets of data. A lot of the common functionalities
 *        and operations that are dimension independent are defined here,
 *        whereas the dimension specific function and operations are relegated
 *        to subclasses.
 *
 */
class DataStorage
{
    protected:
        enum accessors
        {
            x1_accessor = 0,
            x2_accessor = 1,
            x3_accessor = 2
        };

        std::size_t size;
        std::vector<double> data;

        const char same_size_err[45] = "Error: Data objects are not of the same size";
        const char no_dimension_err[45] = "Error: Data object dimension does not exist";

    public:
        /**********************************************************
        ITERATOR FUNCTIONS
        ***********************************************************/
        using iterator = std::vector<double>::iterator;
        using const_iterator = std::vector<double>::const_iterator;

        inline iterator begin() noexcept
        {
            return data.begin();
        }
        inline const_iterator cbegin() const noexcept
        {
            return data.cbegin();
        }
        inline iterator end() noexcept
        {
            return data.end();
        }
        inline const_iterator cend() const noexcept
        {
            return data.cend();
        }
        //-----------------------------------------


        /**********************************************************
        OPERATOR FUNCTIONS
        ***********************************************************/
        // Indexing operator
        /**
         * @brief Returns a reference to the element at specified location idx.
         *        No bounds checking is performed.
         *
         * @param idx Position of the element to return
         * @return double& Reference to the requested element
         */
        inline double& operator[](const std::size_t idx)
        {
            return this->data[idx];
        }

        /**
         * @brief Returns a reference to the element at specified location idx.
         *        No bounds checking is performed.
         *
         * @param idx Position of the element to return
         * @return const double& Reference to the requested element
         */
        inline const double& operator[](const std::size_t idx) const
        {
            return this->data[idx];
        }

        // Printing Operator
        friend std::ostream& operator<<(std::ostream& s, const DataStorage& d)
        {
            return d.print(s);
        }
        //-----------------------------------------


        /**********************************************************
        CLASS METHODS
        ***********************************************************/
        double& at(const std::size_t idx);
        const double& at(const std::size_t idx) const;

        virtual void print() const = 0;
        virtual std::ostream& print(std::ostream& s) const = 0;

        /**
         * @brief Prints the DataStorage object
         *
         * @param d A reference to the DataStorage object to print
         */
        friend void print(const DataStorage& d)
        {
            d.print();
        }

        /**
         * @brief Get the total number of elements in DataStorage
         *
         * @return std::size_t The number of elements in DataStorage
         */
        inline std::size_t get_size() const
        {
            return this->size;
        }

        /**
         * @brief Get a pointer to the data stored in the object
         *
         * @return const double* A pointer to the data
         */
        inline const double* get_data() const
        {
            return this->data.data();
        }

        virtual inline std::size_t get_ndims() const = 0;
        virtual inline std::size_t get_Ni_size(std::size_t i) const = 0;
        //-----------------------------------------
};

#endif
