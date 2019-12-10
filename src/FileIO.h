#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <iostream>
#include <fstream>
#include <H5Cpp.h>

#include "DataStorage.h"
#include "GridObject.h"

//TODO: this should be a singleton
// Have to be very careful here as any path within the hdf5 file that has the same name with throw some sort of group error.
class FileIO
{
    private:
        std::ofstream x_dom, t_dom, dens_out, dens_out_b,
                      dens_out_p, field_out, KE_out, U_out,
                      totE_out, part_x, part_px, part_py;
        H5::H5File file;

        int COMPRESSION_LVL = 6;
        std::size_t NUM_CHUNK = 8;

    public:
        FileIO();
        ~FileIO();

        void open_txt_files();
        void open_hdf5_files(std::string fname);

        void close_txt_files();
        void close_hdf5_files();

        int write_species_to_HDF5(const std::size_t spec_name, const std::size_t itr_num, const DataStorage& data);
        int write_species_to_HDF5(const std::size_t spec_name, const std::size_t itr_num, const GridObject& data);

        int write_e_field_to_HDF5(const std::size_t field_comp, const std::size_t itr_num, const DataStorage& data);
        int write_e_field_to_HDF5(const std::size_t field_comp, const std::size_t itr_num, const GridObject& data);

        int write_b_field_to_HDF5(const std::size_t field_comp, const std::size_t itr_num, const DataStorage &data);
        int write_b_field_to_HDF5(const std::size_t field_comp, const std::size_t itr_num, const GridObject &data);

        int write_phase_to_HDF5(const char phase_name[], const std::size_t spec_name, const std::size_t itr_num, const DataStorage &data);
        int write_phase_to_HDF5(const char phase_name[], const std::size_t spec_name, const std::size_t itr_num, const GridObject &data);
};

#endif
