#ifndef THREEVEC_H
#define THREEVEC_H

#include <cmath>
#include <iostream>

/**
 * @brief Three-vector class including mathematical operations
 *
 */
class ThreeVec
{
    private:
        static const size_t MAX_DIM = 3;
        static const size_t X_IDX = 0;
        static const size_t Y_IDX = 1;
        static const size_t Z_IDX = 2;

        double coord_[MAX_DIM]; // Private data members e.g. x,y,z

    public:
        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        ThreeVec(); // Default constructor
        ThreeVec(double x, double y, double z); // Cartesian constructor
        ~ThreeVec();
        //-----------------------------------------


        /**********************************************************
        OPERATOR FUNCTIONS
        ***********************************************************/
        /**
         * @brief Overload the + operator for vector addition
         *
         * @param vec Vector to perform element-wise addition with
         * @return ThreeVec Resultant ThreeVec
         */
        friend ThreeVec operator+(ThreeVec vec1, const ThreeVec& vec2)
        {
            return vec1 += vec2;
        }

        /**
         * @brief Overload the - operator for vector subtraction
         *
         * @param vec Vector to perform element-wise subtraction with
         * @return ThreeVec Resultant ThreeVec
         */
        friend ThreeVec operator-(ThreeVec vec1, const ThreeVec& vec2)
        {
            return vec1 -= vec2;
        }

        /**
         * @brief Overload the += operator to increment current ThreeVec
         *
         * @param vec Vector to perform in place element-wise addition with
         * @return ThreeVec Resultant ThreeVec
         */
        inline ThreeVec operator+=(const ThreeVec& vec)
        {
            coord_[X_IDX] += vec.get_x();
            coord_[Y_IDX] += vec.get_y();
            coord_[Z_IDX] += vec.get_z();
            return *(this);
        }

        /**
         * @brief Overload the -= operator to decrement current ThreeVec
         *
         * @param vec Vector to perform in place element-wise subtraction with
         * @return ThreeVec Resultant ThreeVec
         */
        inline ThreeVec operator-=(const ThreeVec& vec)
        {
            coord_[X_IDX] -= vec.get_x();
            coord_[Y_IDX] -= vec.get_y();
            coord_[Z_IDX] -= vec.get_z();
            return *(this);
        }

        /**
         * @brief Overload the * operator to perform scalar multiplication
         *
         * @param value Scalar to multiply vector by
         * @return ThreeVec Resultant ThreeVec
         */
        inline ThreeVec operator*(const double value) const
        {
            ThreeVec ans(coord_[X_IDX] * value,
                         coord_[Y_IDX] * value,
                         coord_[Z_IDX] * value);
            return ans;
        }

        /**
         * @brief Overload the / operator to perform scalar division
         *
         * @param value Scalar to divide vector by
         * @return ThreeVec Resultant ThreeVec
         */
        inline ThreeVec operator/(const double value) const
        {
            ThreeVec ans(coord_[X_IDX] / value,
                         coord_[Y_IDX] / value,
                         coord_[Z_IDX] / value);
            return ans;
        }

        /**
         * @brief Overload the * operator to perform a vector dot product
         *
         * @param vec Vector to dot with
         * @return double Resultant dot product
         */
        inline double operator*(const ThreeVec& vec) const
        {
            double ans = 0.0;
            for (std::size_t i = 0; i < MAX_DIM; ++i)
            {
                ans += coord_[i] * vec.get(i);
            }
            return ans;
        }

        /**
         * @brief Overload the ^ operator to perform a vector cross product
         *
         * @param vec Vector to cross with
         * @return ThreeVec Resultant cross product
         */
        inline ThreeVec operator^(ThreeVec vec) const
        {
            ThreeVec ans(coord_[Y_IDX] * vec.get_z() - coord_[Z_IDX] * vec.get_y(),
                         coord_[Z_IDX] * vec.get_x() - coord_[X_IDX] * vec.get_z(),
                         coord_[X_IDX] * vec.get_y() - coord_[Y_IDX] * vec.get_x());
            return ans;
        }
        //-----------------------------------------


        /**********************************************************
        CLASS METHODS
        ***********************************************************/
        // Getter Functions
        /**
         * @brief Access function for x coordinate
         *
         * @return double x coordinate
         */
        inline double get_x() const
        {
            return coord_[X_IDX];
        }

        /**
         * @brief Access function for y coordinate
         *
         * @return double y coordinate
         */
        inline double get_y() const
        {
            return coord_[Y_IDX];
        }

        /**
         * @brief Access function for z coordinate
         *
         * @return double z coordinate
         */
        inline double get_z() const
        {
            return coord_[Z_IDX];
        }

        /**
         * @brief Access function for ith coordinate
         *
         * @param i Index into the vector
         * @return double Value at ith coordinate
         */
        inline double get(int i) const
        {
            return coord_[i];
        }


        // Setter Functions
        /**
         * @brief Modifier method for x coordinate
         *
         * @param value Value to set x coorindate to
         */
        inline void set_x(double value)
        {
            coord_[X_IDX] = value;
        }

        /**
         * @brief Modifier method for y coordinate
         *
         * @param value Value to set y coorindate to
         */
        inline void set_y(double value)
        {
            coord_[Y_IDX] = value;
        }

        /**
         * @brief Modifier method for z coordinate
         *
         * @param value Value to set z coorindate to
         */
        inline void set_z(double value)
        {
            coord_[Z_IDX] = value;
        }

        /**
         * @brief Modifier method for ith coordinate -> INSERT
         *
         * @param i Index into the vector
         * @param value Value to set ith coorindate to
         */
        inline void set(int i, double value)
        {
            coord_[i] = value;
        }

        /**
         * @brief Modifier method for all coordinate
         *
         * @param x New x coordinate
         * @param y New y coordinate
         * @param z New z coordinate
         */
        inline void set_all(double x, double y, double z)
        {
            coord_[X_IDX] = x;
            coord_[Y_IDX] = y;
            coord_[Z_IDX] = z;
        }


        // Operations
        void inc(int i, double value);
        double square();
        double mag();
        ThreeVec element_multiply(const ThreeVec& vec);


        // Print Functions
        void print() const;
        void print_comp(std::size_t i) const;
        //-----------------------------------------
};

#endif
