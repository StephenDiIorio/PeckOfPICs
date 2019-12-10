// #include <cmath>
// #include <iostream>
// #include <fstream>
#include <string>
using namespace std;

#include "FileIO.h"
#include "Simulation.h"
#include "two_stream.h"

int main()
{
    // std::cout << "beginning" << std::endl;
    FileIO io;
    std::string fname = "output.h5";
    io.open_hdf5_files(fname);
    // std::cout << "made file" << std::endl;

    // double ke = 0.0, u = 0.0, tote = 0.0;

    Simulation sim(ndump, Nx, Ny, L_x, L_y, dt, tmax);

    // std::cout << "made sim" << std::endl;

    double t;
    for (t = 0.0; t < sim.tmax; t += sim.dt)
    {
        // std::cout << "in loop" << std::endl;
        // std::cout << t << std::endl;
        // std::cout << sim.nspec << std::endl;
        // sim.print_spec_density(0);
        if (sim.dump_data())
        {
            // std::cout << "print density" << std::endl;
            std::size_t spec_counter = 0;
            for (auto &s : sim.spec)
            {
                // s.density_arr.print();
                // std::cout << "/DENSITY/" + std::to_string(spec_counter) + "/" + std::to_string(sim.n_iter) << std::endl;

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

    // std::cout << "about to close" << std::endl;
    io.close_hdf5_files();
    // std::cout << "closed and exiting" << std::endl;

    return 0;
}
