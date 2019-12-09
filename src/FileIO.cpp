#include "FileIO.h"

FileIO::FileIO()
{
}

FileIO::~FileIO()
{
}

// void FileIO::open_files()
// {
//     #ifdef D_HDF5
//         open_hdf5_files();
//     #else
//         open_txt_files();
//     #endif
// }

// void FileIO::close_files()
// {
//     #ifdef D_HDF5
//         close_hdf5_files();
//     #else
//         close_txt_files();
//     #endif
// }

void FileIO::open_txt_files()
{
    x_dom.open("x_domain.txt");
    t_dom.open("t_domain.txt");
    dens_out.open("dens.txt");
    field_out.open("field.txt");
    KE_out.open("KE.txt");
    U_out.open("U.txt");
    totE_out.open("totE.txt");
    part_x.open("part_x.txt");
    part_px.open("part_px.txt");
    part_py.open("part_py.txt");
}

void FileIO::open_hdf5_files(std::string fname)
{
    // Turn off the auto-printing when failure occurs so that we can
    // handle the errors appropriately
    H5::Exception::dontPrint();

    // H5File *myh5file = NULL;
    H5std_string f_name(fname);
    try
    {
        // Try to open existing file
        this->h5_file = H5::H5File(f_name, H5F_ACC_RDWR);
    }
    catch (H5::FileIException error)
    {
        // Create a new file
        this->h5_file = H5::H5File(f_name, H5F_ACC_TRUNC);
    }
}

void FileIO::close_txt_files()
{
    x_dom.close();
    t_dom.close();
    dens_out.close();
    field_out.close();
    KE_out.close();
    U_out.close();
    totE_out.close();
    part_x.close();
    part_px.close();
    part_py.close();
}

void FileIO::close_hdf5_files()
{
    this->h5_file.close();
}


int FileIO::write_to_hdf5(std::string group_name, std::string ds_name, const DataStorage& data)
{
    H5::Exception::dontPrint();

    // Create/open our group
    H5::Group group_to_write;
    try
    {
        // Create it
        group_to_write = H5::Group(this->h5_file.createGroup("/" + group_name));
    }
    catch (H5::FileIException error)
    {
        // Open it
        group_to_write = H5::Group(this->h5_file.openGroup("/" + group_name));
    }

    try
    {
        hsize_t dim_sizes[data.get_ndims()];
        for (std::size_t i = 0; i < data.get_ndims(); ++i)
        {
            dim_sizes[i] = data.get_Ni_size(i);
        }
        H5::DataSpace myDSpace(data.get_ndims(), dim_sizes);

        H5std_string dsname(ds_name);
        H5::DataSet mydataset = group_to_write.createDataSet(dsname, H5::PredType::NATIVE_DOUBLE, myDSpace);

        mydataset.write(data.get_data(), H5::PredType::NATIVE_DOUBLE);

        mydataset.close();
        myDSpace.close();
    }
    catch (H5::DataSpaceIException error)
    {
        error.printErrorStack();
        return -2;
    }
    catch (H5::DataSetIException error)
    {
        error.printErrorStack();
        return -3;
    }

    group_to_write.close();

    return 0;
}

int FileIO::write_to_hdf5(std::string group_name, std::string ds_name, const GridObject& data)
{
    return write_to_hdf5(group_name, ds_name, data.get_data());
}
