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
        static const uint MAX_DIM = 3;
        static const uint X_IDX = 0;
        static const uint Y_IDX = 1;
        static const uint Z_IDX = 2;

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
        inline ThreeVec operator+(ThreeVec vec)
        {
            ThreeVec ans(coord_[X_IDX] + vec.get_x(),
                         coord_[Y_IDX] + vec.get_y(),
                         coord_[Z_IDX] + vec.get_z());
            return ans;
        }

        /**
         * @brief Overload the - operator for vector subtraction
         *
         * @param vec Vector to perform element-wise subtraction with
         * @return ThreeVec Resultant ThreeVec
         */
        inline ThreeVec operator-(ThreeVec vec)
        {
            ThreeVec ans(coord_[X_IDX] - vec.get_x(),
                         coord_[Y_IDX] - vec.get_y(),
                         coord_[Z_IDX] - vec.get_z());
            return ans;
        }

        /**
         * @brief Overload the += operator to increment current ThreeVec
         *
         * @param vec Vector to perform in place element-wise addition with
         * @return ThreeVec Resultant ThreeVec
         */
        inline ThreeVec operator+=(ThreeVec vec)
        {
            coord_[X_IDX] += vec.get_x();
            coord_[Y_IDX] += vec.get_y();
            coord_[Z_IDX] += vec.get_z();
        }

        /**
         * @brief Overload the -= operator to decrement current ThreeVec
         *
         * @param vec Vector to perform in place element-wise subtraction with
         * @return ThreeVec Resultant ThreeVec
         */
        inline ThreeVec operator-=(ThreeVec vec)
        {
            coord_[X_IDX] -= vec.get_x();
            coord_[Y_IDX] -= vec.get_y();
            coord_[Z_IDX] -= vec.get_z();
        }

        /**
         * @brief Overload the * operator to perform scalar multiplication
         *
         * @param value Scalar to multiply vector by
         * @return ThreeVec Resultant ThreeVec
         */
        inline ThreeVec operator*(double value)
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
        inline ThreeVec operator/(double value)
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
        inline double operator*(ThreeVec vec)
        {
            double ans = 0.0;
            for (int i = 0; i < MAX_DIM; ++i)
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
        inline ThreeVec operator^(ThreeVec vec)
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
        double get_x();
        double get_y();
        double get_z();
        double get(int i);

        void set_x(double value);
        void set_y(double value);
        void set_z(double value);
        void set(int i, double value);
        void set_all(double x, double y, double z);

        void inc(int i, double value);
        double square();
        double mag();

        ThreeVec element_multiply(ThreeVec vec);
        //-----------------------------------------
};

#endif
