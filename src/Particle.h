#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>

#include "ThreeVec.h"

/**
 * @brief A particle class with all of the attributes a particle can have
 *
 */
class Particle
{
    private:
        ThreeVec pos;
        ThreeVec mom;
        ThreeVec local_e_field;
        ThreeVec local_b_field;

        double weight;

    public:
        /**********************************************************
        CONSTRUCTORS/DESTRUCTORS
        ***********************************************************/
        Particle();
        Particle(double weight);
        Particle(double pos_x, double pos_y, double pos_z,
                 double mom_x, double mom_y, double mom_z);
        Particle(double pos_x, double pos_y, double pos_z,
                 double mom_x, double mom_y, double mom_z,
                 double weight);
        Particle(const ThreeVec& pos, const ThreeVec& mom);
        Particle(const ThreeVec& pos, const ThreeVec& mom, double weight);
        ~Particle();
        //-----------------------------------------


        /**********************************************************
        CLASS METHODS
        ***********************************************************/
        // Getter Functions
        /**
         * @brief Get the vector containing the position of the particle
         *
         * @return ThreeVec The ThreeVec containing the particle position
         */
        inline ThreeVec get_pos() const
        {
            return this->pos;
        }

        /**
         * @brief Get a component of the position of the particle
         *
         * @param i Index to component to retrieve
         * @return double The desired component
         */
        inline double get_pos_comp(std::size_t i) const
        {
            return this->pos.get(i);
        }

        /**
         * @brief Get the vector containing the momentum of the particle
         *
         * @return ThreeVec The ThreeVec containing the particle momentum
         */
        inline ThreeVec get_mom() const
        {
            return this->mom;
        }

        /**
         * @brief Get a component of the momentum of the particle
         *
         * @param i Index to component to retrieve
         * @return double The desired component
         */
        inline double get_mom_comp(std::size_t i) const
        {
            return this->mom.get(i);
        }

        /**
         * @brief Get the weight of the particle
         *
         * @return double The weight of the particle
         */
        inline double get_weight() const
        {
            return this->weight;
        }

        /**
         * @brief Get vector containing the local electric field the particle
         *        sees
         *
         * @return ThreeVec The ThreeVec containing the local electric field
         */
        inline ThreeVec get_local_e_field() const
        {
            return this->local_e_field;
        }

        /**
         * @brief Get a component of the local electric field of the particle
         *
         * @param i Index to component to retrieve
         * @return double The desired component
         */
        inline double get_local_e_field_comp(std::size_t i) const
        {
            return this->local_e_field.get(i);
        }

        /**
         * @brief Get vector containing the local magnetic field the particle
         *        sees
         *
         * @return ThreeVec The ThreeVec containing the local magnetic field
         */
        inline ThreeVec get_local_b_field() const
        {
            return this->local_b_field;
        }

        /**
         * @brief Get the local b field comp object
         *
         * @param i Index to component to retrieve
         * @return double The desired component
         */
        inline double get_local_b_field_comp(std::size_t i) const
        {
            return this->local_b_field.get(i);
        }


        // Setter Functions
        /**
         * @brief Copy values from a vector to set the particle position
         *
         * @param pos The ThreeVec with position values to copy to current
         *            particle
         */
        inline void set_pos(const ThreeVec& pos)
        {
            this->set_pos(pos.get_x(), pos.get_y(), pos.get_z());
        }

        /**
         * @brief Set a component of the particle position
         *
         * @param i Index to component to set
         * @param val Value to set component
         */
        inline void set_pos_comp(std::size_t i, const double val)
        {
            this->pos.set(i, val);
        }

        /**
         * @brief Sets the position components to provided values
         *
         * @param pos_x Value to set x coordinate
         * @param pos_y Value to set y coordinate
         * @param pos_z Value to set z coordinate
         */
        inline void set_pos(const double pos_x,
                            const double pos_y,
                            const double pos_z)
        {
            this->pos.set_all(pos_x, pos_y, pos_z);
        }

        /**
         * @brief Copy values from a vector to set the particle momentum
         *
         * @param mom The ThreeVec with momentum values to copy to current
         *            particle
         */
        inline void set_mom(const ThreeVec& mom)
        {
            this->set_mom(mom.get_x(), mom.get_y(), mom.get_z());
        }

        /**
         * @brief Set a component of the particle momentum
         *
         * @param i Index to component to set
         * @param val Value to set component
         */
        inline void set_mom_comp(std::size_t i, const double val)
        {
            this->mom.set(i, val);
        }

        /**
         * @brief Sets the momentum components to provided values
         *
         * @param mom_x Value to set x coordinate
         * @param mom_y Value to set y coordinate
         * @param mom_z Value to set z coordinate
         */
        inline void set_mom(const double mom_x,
                            const double mom_y,
                            const double mom_z)
        {
            this->mom.set_all(mom_x, mom_y, mom_z);
        }

        /**
         * @brief Set the weight of the particle
         *
         * @param weight Value to set the weight of the particle
         */
        inline void set_weight(const double weight)
        {
            this->weight = weight;
        }

        /**
         * @brief Copy values from a vector to set the local electric field the
         *        particle sees
         *
         * @param field The ThreeVec with electric field vaules to copy to the
         *              local copy
         */
        inline void set_local_e_field(const ThreeVec& field)
        {
            this->set_local_e_field(field.get_x(),
                                    field.get_y(),
                                    field.get_z());
        }

        /**
         * @brief Set a component of the local electric field of the particle
         *
         * @param i Index to component to set
         * @param val Value to set component
         */
        inline void set_local_e_field_comp(std::size_t i, const double val)
        {
            this->local_e_field.set(i, val);
        }

        /**
         * @brief Sets the local electric field the particle sees to the
         *        provided values
         *
         * @param x1 Value to set x1 component
         * @param x2 Value to set x2 component
         * @param x3 Value to set x3 component
         */
        inline void set_local_e_field(const double x1,
                                      const double x2,
                                      const double x3)
        {
            this->local_e_field.set_all(x1, x2, x3);
        }

        /**
         * @brief Copy values from a vector to set the local magnetic field the
         *        particle sees
         *
         * @param field The ThreeVec with electric field vaules to copy to the
         *              local copy
         */
        inline void set_local_b_field(const ThreeVec& field)
        {
            this->set_local_b_field(field.get_x(),
                                    field.get_y(),
                                    field.get_z());
        }

        /**
         * @brief Set a component of the local magnetic field of the particle
         *
         * @param i Index to component to set
         * @param val Value to set component
         */
        inline void set_local_b_field_comp(std::size_t i, const double val)
        {
            this->local_b_field.set(i, val);
        }

        /**
         * @brief Sets the local magnetic field the particle sees to the
         *        provided values
         *
         * @param x1 Value to set x1 component
         * @param x2 Value to set x2 component
         * @param x3 Value to set x3 component
         */
        inline void set_local_b_field(const double x1,
                                      const double x2,
                                      const double x3)
        {
            this->local_b_field.set_all(x1, x2, x3);
        }


        // Print Functions
        void print_pos() const;
        void print_pos_comp(std::size_t i) const;

        void print_mom() const;
        void print_mom_comp(std::size_t i) const;

        void print_weight() const;

        void print_local_e_field() const;
        void print_local_e_field_comp(std::size_t i) const;

        void print_local_b_field() const;
        void print_local_b_field_comp(std::size_t i) const;
        //-----------------------------------------
};

#endif
