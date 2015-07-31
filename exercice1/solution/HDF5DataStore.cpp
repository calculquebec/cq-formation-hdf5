#include "HDF5DataStore.h"

#include <exception>
#include <boost/filesystem.hpp>

using namespace std;

HDF5DataStore::HDF5DataStore(const string & in_filename, const bool readOnly, const bool truncate)
: _readOnly(readOnly), _truncate(truncate), _filename(in_filename)
{
    openFile();
}

HDF5DataStore::~HDF5DataStore()
{
    closeFile();
}

void HDF5DataStore::createFile()
{
    _is_closed = false;
    if (_truncate)
        _file_id = H5Fcreate(_filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    else
        _file_id = H5Fcreate(_filename.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
    closeFile();
}

void HDF5DataStore::openFile()
{
    boost::filesystem::path p(_filename);
    if (! _readOnly && (_truncate || !boost::filesystem::exists(p)))
        createFile();

    if (_readOnly)
        _file_id = H5Fopen(_filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    else
        _file_id = H5Fopen(_filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
    _is_closed = false;
}

void HDF5DataStore::closeFile()
{
    if (! _is_closed) {
        H5Fclose(_file_id);
        _is_closed = true;
    }
}

void HDF5DataStore::set(const char * name, const double * arr, size_t total_size[2])
{
	const int NDIM=2;
	hid_t dset_id;
	hid_t filespace, memspace;
	hsize_t file_size[NDIM], mem_size[NDIM];

    // Set all the correct dimensions
    file_size[0] = total_size[0]; 	file_size[1] = total_size[1];
    mem_size[0] = total_size[0]; 	mem_size[1] = total_size[1];

	// Create the filespace and dataset
    filespace = H5Screate_simple(NDIM, file_size, NULL); 
    dset_id = H5Dcreate(_file_id, name, H5T_NATIVE_DOUBLE, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	// Create the memory space
    memspace = H5Screate_simple(NDIM, mem_size, NULL);

	// Write data
	_status = H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, memspace, filespace, H5P_DEFAULT, arr);

	// Close resources
	H5Dclose(dset_id);
    H5Sclose(filespace);
    H5Sclose(memspace);
}
void HDF5DataStore::get(const char * name, double * arr, size_t total_size[2])
{
	const int NDIM=2;
	hid_t dset_id;
	hid_t filespace, memspace;
	hsize_t file_size[NDIM], mem_size[NDIM];

	// Set all the correct dimensions
    file_size[0] = total_size[0]; 	file_size[1] = total_size[1];
    mem_size[0] = total_size[0]; 	mem_size[1] = total_size[1];

	// Open the dataset and retrieve the space
    dset_id = H5Dopen(_file_id, name, H5P_DEFAULT);
    filespace = H5Dget_space(dset_id);
	// Create the memory space
    memspace = H5Screate_simple(NDIM, mem_size, NULL);
	// Read the data
	_status = H5Dread(dset_id, H5T_NATIVE_DOUBLE, memspace, filespace, H5P_DEFAULT, arr);

	// Close resources.
	H5Dclose(dset_id);
    H5Sclose(filespace);
    H5Sclose(memspace);
}
void HDF5DataStore::set(const char * name, const double * arr, size_t total_size)
{
	const int NDIM=1;
	hid_t dset_id;
	hid_t filespace, memspace;
	hsize_t file_size[NDIM], mem_size[NDIM];
	// Set all the correct dimensions
    file_size[0] = total_size;
    mem_size[0] = total_size;

	// Create the filespace and dataset
    filespace = H5Screate_simple(NDIM, file_size, NULL); 
    dset_id = H5Dcreate(_file_id, name, H5T_NATIVE_DOUBLE, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // Create the memory space
    memspace = H5Screate_simple(NDIM, mem_size, NULL);

	// Write data
	_status = H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, memspace, filespace, H5P_DEFAULT, arr);

	// Close resources
	H5Dclose(dset_id);
    H5Sclose(filespace);
    H5Sclose(memspace);
}
void HDF5DataStore::get(const char * name, double * arr, size_t total_size)
{
	const int NDIM=1;
	hid_t dset_id;
	hid_t filespace, memspace;
	hsize_t file_size[NDIM], mem_size[NDIM];
	// Set all the correct dimensions
    file_size[0] = total_size;
    mem_size[0] = total_size;
	// Open the dataset and retrieve the space
    dset_id = H5Dopen(_file_id, name, H5P_DEFAULT);
    filespace = H5Dget_space(dset_id);
	// Create the memory space
    memspace = H5Screate_simple(NDIM, mem_size, NULL);

	// Read the data
	_status = H5Dread(dset_id, H5T_NATIVE_DOUBLE, memspace, filespace, H5P_DEFAULT, arr);

	// Close resources.
	H5Dclose(dset_id);
    H5Sclose(filespace);
    H5Sclose(memspace);
}


