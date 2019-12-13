// #include <cmath>
// #include <iostream>
// #include <fstream>
#include <string>
using namespace std;

#include "FileIO.h"
#include "Simulation.h"
#include "two_part_test.h"

int main()
{
    FileIO io;
    std::string fname = "output.h5";
    io.open_hdf5_files(fname);

    // double ke = 0.0, u = 0.0, tote = 0.0;

    Simulation sim(ndump, Nx, Ny, L_x, L_y, dt, tmax);

    // std::cout << "Made sim" << std::endl;
    // std::cout << sim.spec[0].density << std::endl;
    // std::cout << sim.spec[0].npar << std::endl;
    // std::cout << sim.spec[0].Qpar << std::endl;
    // sim.spec[0].print_pos();
    // sim.spec[0].print_weight();

    double t;
    for (t = 0.0; t < sim.tmax; t += sim.dt)
    {
        std::cout << "t= \t" << t << std::endl;

        if (sim.dump_data())
        {
            std::size_t spec_counter = 0;
            for (auto &s : sim.spec)
            {
                io.write_species_to_HDF5(spec_counter, sim.n_iter, s.density_arr);
                ++spec_counter;
            }

            io.write_e_field_to_HDF5(1, sim.n_iter, sim.e_field.f1);
            io.write_e_field_to_HDF5(2, sim.n_iter, sim.e_field.f2);
            io.write_e_field_to_HDF5(3, sim.n_iter, sim.e_field.f3);

            io.write_b_field_to_HDF5(1, sim.n_iter, sim.b_field.f1);
            io.write_b_field_to_HDF5(2, sim.n_iter, sim.b_field.f2);
            io.write_b_field_to_HDF5(3, sim.n_iter, sim.b_field.f3);

            //TODO: energy calculations and output

            spec_counter = 0;
            for (auto &s : sim.spec)
            {
                io.write_phase_to_HDF5("X", spec_counter, sim.n_iter, s.get_x_phasespace());
                io.write_phase_to_HDF5("Y", spec_counter, sim.n_iter, s.get_y_phasespace());
                io.write_phase_to_HDF5("PX", spec_counter, sim.n_iter, s.get_px_phasespace());
                io.write_phase_to_HDF5("PY", spec_counter, sim.n_iter, s.get_py_phasespace());
                ++spec_counter;
            }

            //TODO: output time data as attribute or its own thing?
        }

        sim.iterate();
    }

    io.close_hdf5_files();
    std::cout << "closed and exiting" << std::endl;

    return 0;
}
