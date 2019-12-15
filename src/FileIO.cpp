#include "FileIO.h"

/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/

/**
 * @brief Constructor for the FileIO object
 *
 */
FileIO::FileIO()
{
}

/**
 * @brief Destructor for FileIO object
 *
 */
FileIO::~FileIO()
{
}
//-----------------------------------------


/**********************************************************
CLASS METHODS
***********************************************************/

/**
 * @brief Opens all of the necessary text files to write data in .txt format
 *
 */
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

/**
 * @brief Opens the HDF5 file to write the data to
 *
 * @param fname The string containing the name to call the output file
 */
void FileIO::open_hdf5_files(std::string fname)
{
    // Turn off the auto-printing when failure occurs so that we can
    // handle the errors appropriately
    H5::Exception::dontPrint(); // Set globally

    H5std_string f_name(fname);
    // Always overwrite the old file
    file = H5::H5File(f_name, H5F_ACC_TRUNC);
}

/**
 * @brief Closes all of the .txt files that were opened
 *
 */
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

/**
 * @brief Closes the .hdf5 files that were opened
 *
 */
void FileIO::close_hdf5_files()
{
    file.close();
}


/**
 * @brief Writes a Species's density to an HDF5 file
 *
 * @param spec_name The name/identifier of the species
 * @param itr_num The simulation iteration number
 * @param data The DataStorage object to write to file
 * @return int An error code if something failed, otherwise 0
 */
int FileIO::write_species_to_HDF5(const std::size_t spec_name, const std::size_t itr_num, const DataStorage& data)
{
    H5::Group top_group;
    H5std_string dens_gname("/DENSITY/");
    try
    {
        top_group = H5::Group(file.createGroup(dens_gname));
    }
    catch(H5::FileIException error)
    {
        top_group = H5::Group(file.openGroup(dens_gname));
    }

    H5::Group spec_group;
    H5std_string spec_gname(std::to_string(spec_name));
    try
    {
        spec_group = H5::Group(top_group.createGroup(spec_gname));
    }
    catch (H5::GroupIException error)
    {
        spec_group = H5::Group(top_group.openGroup(spec_gname));
    }

    try
    {
        hsize_t dim_sizes[data.get_ndims()];
        hsize_t chunk_dims[data.get_ndims()];
        hsize_t chunk_size;
        for (std::size_t i = 0; i < data.get_ndims(); ++i)
        {
            dim_sizes[i] = data.get_Ni_size(i);
            chunk_size = data.get_Ni_size(i) / NUM_CHUNK;
            if (chunk_size == 0)
            {
                chunk_size = 1;
            }
            chunk_dims[i] = chunk_size;
        }
        H5::DSetCreatPropList *plist = new H5::DSetCreatPropList;
        plist->setChunk(data.get_ndims(), chunk_dims);
        plist->setDeflate(COMPRESSION_LVL);

        H5::DataSpace spec_ds(data.get_ndims(), dim_sizes);
        H5std_string spec_dsname(std::to_string(itr_num));
        H5::DataSet spec_dataset = spec_group.createDataSet(spec_dsname, H5::PredType::NATIVE_DOUBLE, spec_ds, *plist);

        spec_dataset.write(data.get_data(), H5::PredType::NATIVE_DOUBLE);

        spec_dataset.close();
        spec_ds.close();
        delete plist;
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

    spec_group.close();
    top_group.close();

    return 0;
}

/**
 * @brief Writes a Species's density to an HDF5 file
 *
 * @param spec_name The name/identifier of the species
 * @param itr_num The simulation iteration number
 * @param data The GridObject object to write to file
 * @return int An error code if something failed, otherwise 0
 */
int FileIO::write_species_to_HDF5(const std::size_t spec_name, const std::size_t itr_num, const GridObject& data)
{
    return write_species_to_HDF5(spec_name, itr_num, data.get_data());
}

/**
 * @brief Writes a Electric Field components to file
 *
 * @param field_comp The identifier for the component of the field
 * @param itr_num The simulation iteration number
 * @param data The DataStorage object to write to file
 * @return int An error code if something failed, otherwise 0
 */
int FileIO::write_e_field_to_HDF5(const std::size_t field_comp, const std::size_t itr_num, const DataStorage& data)
{
    H5::Group top_group;
    H5std_string f_gname("/E_FIELD/");
    try
    {
        top_group = H5::Group(file.createGroup(f_gname));
    }
    catch(H5::FileIException error)
    {
        top_group = H5::Group(file.openGroup(f_gname));
    }

    H5::Group comp_group;
    H5std_string comp_gname("x" + std::to_string(field_comp));
    try
    {
        comp_group = H5::Group(top_group.createGroup(comp_gname));
    }
    catch (H5::GroupIException error)
    {
        comp_group = H5::Group(top_group.openGroup(comp_gname));
    }

    try
    {
        hsize_t dim_sizes[data.get_ndims()];
        hsize_t chunk_dims[data.get_ndims()];
        hsize_t chunk_size;
        for (std::size_t i = 0; i < data.get_ndims(); ++i)
        {
            dim_sizes[i] = data.get_Ni_size(i);
            chunk_size = data.get_Ni_size(i) / NUM_CHUNK;
            if (chunk_size == 0)
            {
                chunk_size = 1;
            }
            chunk_dims[i] = chunk_size;
        }
        H5::DSetCreatPropList *plist = new H5::DSetCreatPropList;
        plist->setChunk(data.get_ndims(), chunk_dims);
        plist->setDeflate(COMPRESSION_LVL);

        H5::DataSpace f_ds(data.get_ndims(), dim_sizes);
        H5std_string f_dsname(std::to_string(itr_num));
        H5::DataSet f_dataset = comp_group.createDataSet(f_dsname, H5::PredType::NATIVE_DOUBLE, f_ds, *plist);

        f_dataset.write(data.get_data(), H5::PredType::NATIVE_DOUBLE);

        f_dataset.close();
        f_ds.close();
        delete plist;
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

    comp_group.close();
    top_group.close();

    return 0;
}

/**
 * @brief Writes a Electric Field components to file
 *
 * @param field_comp The identifier for the component of the field
 * @param itr_num The simulation iteration number
 * @param data The GridObject object to write to file
 * @return int An error code if something failed, otherwise 0
 */
int FileIO::write_e_field_to_HDF5(const std::size_t field_comp, const std::size_t itr_num, const GridObject& data)
{
    return write_e_field_to_HDF5(field_comp, itr_num, data.get_data());
}


/**
 * @brief Writes a Magnetic Field components to file
 *
 * @param field_comp The identifier for the component of the field
 * @param itr_num The simulation iteration number
 * @param data The DataStorage object to write to file
 * @return int An error code if something failed, otherwise 0
 */
int FileIO::write_b_field_to_HDF5(const std::size_t field_comp, const std::size_t itr_num, const DataStorage& data)
{
    H5::Group top_group;
    H5std_string f_gname("/B_FIELD/");
    try
    {
        top_group = H5::Group(file.createGroup(f_gname));
    }
    catch (H5::FileIException error)
    {
        top_group = H5::Group(file.openGroup(f_gname));
    }

    H5::Group comp_group;
    H5std_string comp_gname("x" + std::to_string(field_comp));
    try
    {
        comp_group = H5::Group(top_group.createGroup(comp_gname));
    }
    catch (H5::GroupIException error)
    {
        comp_group = H5::Group(top_group.openGroup(comp_gname));
    }

    try
    {
        hsize_t dim_sizes[data.get_ndims()];
        hsize_t chunk_dims[data.get_ndims()];
        hsize_t chunk_size;
        for (std::size_t i = 0; i < data.get_ndims(); ++i)
        {
            dim_sizes[i] = data.get_Ni_size(i);
            chunk_size = data.get_Ni_size(i) / NUM_CHUNK;
            if (chunk_size == 0)
            {
                chunk_size = 1;
            }
            chunk_dims[i] = chunk_size;
        }
        H5::DSetCreatPropList *plist = new H5::DSetCreatPropList;
        plist->setChunk(data.get_ndims(), chunk_dims);
        plist->setDeflate(COMPRESSION_LVL);

        H5::DataSpace f_ds(data.get_ndims(), dim_sizes);
        H5std_string f_dsname(std::to_string(itr_num));
        H5::DataSet f_dataset = comp_group.createDataSet(f_dsname, H5::PredType::NATIVE_DOUBLE, f_ds, *plist);

        f_dataset.write(data.get_data(), H5::PredType::NATIVE_DOUBLE);

        f_dataset.close();
        f_ds.close();
        delete plist;
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

    comp_group.close();
    top_group.close();

    return 0;
}

/**
 * @brief Writes a Magnetic Field components to file
 *
 * @param field_comp The identifier for the component of the field
 * @param itr_num The simulation iteration number
 * @param data The GridObject object to write to file
 * @return int An error code if something failed, otherwise 0
 */
int FileIO::write_b_field_to_HDF5(const std::size_t field_comp, const std::size_t itr_num, const GridObject& data)
{
    return write_b_field_to_HDF5(field_comp, itr_num, data.get_data());
}


/**
 * @brief Writes a species phase space to file
 *
 * @param phase_name The name of the phase space being written
 * @param spec_name The name/identifier of the species
 * @param itr_num itr_num The simulation iteration number
 * @param data The DataStorage object to write to file
 * @return int An error code if something failed, otherwise 0
 */
int FileIO::write_phase_to_HDF5(const char phase_name[], const std::size_t spec_name, const std::size_t itr_num, const DataStorage& data)
{
    H5::Group top_group;
    H5std_string p_gname("/PHASE/");
    try
    {
        top_group = H5::Group(file.createGroup(p_gname));
    }
    catch (H5::FileIException error)
    {
        top_group = H5::Group(file.openGroup(p_gname));
    }

    H5::Group ptype_group;
    H5std_string ptype_gname(phase_name);
    try
    {
        ptype_group = H5::Group(top_group.createGroup(ptype_gname));
    }
    catch (H5::GroupIException error)
    {
        ptype_group = H5::Group(top_group.openGroup(ptype_gname));
    }

    H5::Group spec_group;
    H5std_string spec_gname(std::to_string(spec_name));
    try
    {
        spec_group = H5::Group(ptype_group.createGroup(spec_gname));
    }
    catch (H5::GroupIException error)
    {
        spec_group = H5::Group(ptype_group.openGroup(spec_gname));
    }

    try
    {
        hsize_t dim_sizes[data.get_ndims()];
        hsize_t chunk_dims[data.get_ndims()];
        hsize_t chunk_size;
        for (std::size_t i = 0; i < data.get_ndims(); ++i)
        {
            dim_sizes[i] = data.get_Ni_size(i);
            chunk_size = data.get_Ni_size(i) / NUM_CHUNK;
            if (chunk_size == 0)
            {
                chunk_size = 1;
            }
            chunk_dims[i] = chunk_size;
        }
        H5::DSetCreatPropList *plist = new H5::DSetCreatPropList;
        plist->setChunk(data.get_ndims(), chunk_dims);
        plist->setDeflate(COMPRESSION_LVL);

        H5::DataSpace p_ds(data.get_ndims(), dim_sizes);
        H5std_string p_dsname(std::to_string(itr_num));
        H5::DataSet p_dataset = spec_group.createDataSet(p_dsname, H5::PredType::NATIVE_DOUBLE, p_ds, *plist);

        p_dataset.write(data.get_data(), H5::PredType::NATIVE_DOUBLE);

        p_dataset.close();
        p_ds.close();
        delete plist;
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

    spec_group.close();
    ptype_group.close();
    top_group.close();

    return 0;
}

/**
 * @brief Writes a species phase space to file
 *
 * @param phase_name The name of the phase space being written
 * @param spec_name The name/identifier of the species
 * @param itr_num itr_num The simulation iteration number
 * @param data The GridObject object to write to file
 * @return int An error code if something failed, otherwise 0
 */
int FileIO::write_phase_to_HDF5(const char phase_name[], const std::size_t spec_name, const std::size_t itr_num, const GridObject& data)
{
    return write_phase_to_HDF5(phase_name, spec_name, itr_num, data.get_data());
}
//-----------------------------------------
