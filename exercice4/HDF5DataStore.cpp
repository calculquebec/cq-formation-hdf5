#include "HDF5DataStore.h"

#include <exception>
#include <boost/filesystem.hpp>

using namespace std;

void HDF5DataStore::createMPIInfo()
{
	//
	// TODO
	// Initialize the _info structure (of MPI_Info type) with MPI_Info_create
	// Signature of the function:
	// int MPI_Info_create(MPI_Info *info);
	// Note: _info is a member structure declared in HDF5DataStore.h
	int status = MPI_Info_create(...);
}

void HDF5DataStore::freeMPIInfo()
{
	//
	// TODO
	// Free the _info structure with MPI_Info_free
	// Signature of the function:
	// int MPI_Info_free( MPI_Info *info )
	//
	MPI_Info_free(...);
}

void HDF5DataStore::createAccessPList()
{
	//
	// TODO
	// Create the access property list _fapl with H5Pcreate and use the function H5Pset_fapl_mpio to set the access
	// to use MPI-IO. Use the _info variable and MPI_COMM_WORLD.
	// Signature of the functions:
	// hid_t H5Pcreate( hid_t cls_id )
	// herr_t H5Pset_fapl_mpio(hid_t fapl_id, MPI_Comm comm, MPI_Info info )
	// Note 1: _fapl is a member ID declared in HDF5DataStore.h
	//
	_fapl = H5Pcreate(...);
	H5Pset_fapl_mpio(...);

}
void HDF5DataStore::closeAccessPList()
{
	//
	// TODO
	// Close the _fapl property list
	// Signature of the function:
	// herr_t H5Pclose(hid_t plist ) 
	H5Pclose(...);
}

void HDF5DataStore::setAttribute(const char * name, const char * val, const char * dset_name)
{
    H5LTset_attribute_string(_file_id, dset_name, name, val);
}

string HDF5DataStore::getAttribute(const char * name, const char * dset_name)
{
    hsize_t dims;
    H5T_class_t type_class;
    size_t attr_size;
    string res;
    if (H5LTget_attribute_info(_file_id, dset_name, name, &dims, &type_class, &attr_size) >= 0)
    {
        char * val = new char[attr_size];
        H5LTget_attribute_string(_file_id, dset_name, name, val);
        res = string(val);
        delete[] val;
    }
    return res;
}

void HDF5DataStore::createGroup(const char * name)
{
    hid_t g = H5Gcreate(_file_id, name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Gclose(g);
}

HDF5DataStore::HDF5DataStore(const string & in_filename, const bool readOnly, const bool truncate)
: _readOnly(readOnly), _truncate(truncate), _filename(in_filename)
{
	createMPIInfo();
	createAccessPList();
    openFile();
}

HDF5DataStore::~HDF5DataStore()
{
	close();
}
void HDF5DataStore::close() {
	if (! _is_closed) {
	    closeFile();
		closeAccessPList();
		freeMPIInfo();
		_is_closed = true;
	}
}

void HDF5DataStore::createFile()
{
    _is_closed = false;
	//
	// TODO
	// Use the _fapl property list instead of H5P_DEFAULT for the access property list.
	//
    if (_truncate)
        _file_id = H5Fcreate(_filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT ...);
    else
        _file_id = H5Fcreate(_filename.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT ...);
    closeFile();
}

void HDF5DataStore::openFile()
{
    boost::filesystem::path p(_filename);
    if (! _readOnly && (_truncate || !boost::filesystem::exists(p)))
        createFile();
	//
	// TODO
	// Use the _fapl property list instead of H5P_DEFAULT for the access property list.
	//
    if (_readOnly)
        _file_id = H5Fopen(_filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT ...);
    else
        _file_id = H5Fopen(_filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT ...);
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


