#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <iostream>
#include <fstream>
#include <H5Cpp.h>

#include "DataStorage.h"
#include "GridObject.h"

//TODO: this should be a singleton
class FileIO
{
    private:
        std::ofstream x_dom, t_dom, dens_out, dens_out_b,
                      dens_out_p, field_out, KE_out, U_out,
                      totE_out, part_x, part_px, part_py;
        H5::H5File h5_file;

    public:
        FileIO();
        ~FileIO();

        // void open_files();
        // void close_files();

        void open_txt_files();
        void open_hdf5_files(std::string fname);

        void close_txt_files();
        void close_hdf5_files();

        int write_to_hdf5(std::string group_name, std::string ds_name, const DataStorage& data);
        int write_to_hdf5(std::string group_name, std::string ds_name, const GridObject& data);
};

#endif
