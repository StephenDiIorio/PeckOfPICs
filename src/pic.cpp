// #include <cmath>
#include <iostream>
// #include <fstream>
#include <string>
using namespace std;

#include "FileIO.h"
#include "Simulation.h"
#include "two_stream.h"

int main()
{
    std::cout << "beginning" << std::endl;
    FileIO io;
    std::string fname = "output.h5";
    io.open_hdf5_files(fname);
    std::cout << "made file" << std::endl;

    // double ke = 0.0, u = 0.0, tote = 0.0;

    Simulation sim(ndump, Nx, Ny, L_x, L_y, dt, tmax);

    std::cout << "made sim" << std::endl;

    double t;
    for (t = 0.0; t < sim.tmax; t += sim.dt)
    {
        std::cout << "in loop" << std::endl;
        std::cout << t << std::endl;
        if (sim.dump_data())
        {
            std::cout << "print density" << std::endl;
            std::size_t spec_counter = 0;
            for (auto &s : sim.spec)
            {
                // s.density_arr.print();
                io.write_to_hdf5("DENSITY/" + std::to_string(spec_counter), std::to_string(sim.n_iter), s.density_arr);
                ++spec_counter;
            }

            // std::cout << "print field e1" << std::endl;
            // io.write_to_hdf5("FIELD/E1", std::to_string(sim.n_iter), sim.e_field.f1);
            // std::cout << "print field e2" << std::endl;
            // io.write_to_hdf5("FIELD/E2", std::to_string(sim.n_iter), sim.e_field.f2);
            // std::cout << "print field e3" << std::endl;
            // io.write_to_hdf5("FIELD/E3", std::to_string(sim.n_iter), sim.e_field.f3);
            // std::cout << "print field b1" << std::endl;
            // io.write_to_hdf5("FIELD/B1", std::to_string(sim.n_iter), sim.b_field.f1);
            // std::cout << "print field b2" << std::endl;
            // io.write_to_hdf5("FIELD/B2", std::to_string(sim.n_iter), sim.b_field.f2);
            // std::cout << "print field b3" << std::endl;
            // io.write_to_hdf5("FIELD/B3", std::to_string(sim.n_iter), sim.b_field.f3);

            //TODO: energy calculations and output

            // for (uint i = 0; i < sim.nspec; ++i)
            // {
            //     phase = sim.spec.at(i).get_x_phasespace();
            //     for (uint j = 0; j < sim.spec.at(i).npar; ++j)
            //     {
            //         part_x << phase[j] << '\t';
            //     }
            //     phase = sim.spec.at(i).get_px_phasespace();
            //     for (uint j = 0; j < sim.spec.at(i).npar; ++j)
            //     {
            //         part_px << phase[j] << '\t';
            //     }
            //     phase = sim.spec.at(i).get_py_phasespace();
            //     for (uint j = 0; j < sim.spec.at(i).npar; ++j)
            //     {
            //         part_py << phase[j] << '\t';
            //     }
            // }
            // part_x << '\n';
            // part_px << '\n';
            // part_py << '\n';
            std::cout << "print phase" << std::endl;
            spec_counter = 0;
            for (auto &s : sim.spec)
            {
                io.write_to_hdf5("PHASE/X" + std::to_string(spec_counter), std::to_string(sim.n_iter), s.get_x_phasespace());
                io.write_to_hdf5("PHASE/Y" + std::to_string(spec_counter), std::to_string(sim.n_iter), s.get_y_phasespace());
                io.write_to_hdf5("PHASE/PX" + std::to_string(spec_counter), std::to_string(sim.n_iter), s.get_px_phasespace());
                io.write_to_hdf5("PHASE/PY" + std::to_string(spec_counter), std::to_string(sim.n_iter), s.get_py_phasespace());
                ++spec_counter;
            }

            //TODO: output time data as attribute or its own thing
        }

        sim.iterate();
    }

    std::cout << "about to close" << std::endl;
    io.close_hdf5_files();
    std::cout << "closed and exiting" << std::endl;

    return 0;
}
