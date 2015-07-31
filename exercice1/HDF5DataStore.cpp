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
	//
	// TODO
	// Call H5Fcreate with the correct parameters
	//
	// Signature of the function :
	//
	// hid_t H5Fcreate( const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id )
	//
	// Hint 1: To convert _filename to a char *, use _filename.c_str()
	// Hint 2: You can use H5P_DEFAULT for properties
	// Hint 3: The possible flags are H5F_ACC_TRUNC, H5F_ACC_EXCL, H5F_ACC_RDONLY, H5F_ACC_RDWR
	//
    if (_truncate)
        _file_id = H5Fcreate(...);
    else
        _file_id = H5Fcreate(...);
    closeFile();
}

void HDF5DataStore::openFile()
{
    boost::filesystem::path p(_filename);
    if (! _readOnly && (_truncate || !boost::filesystem::exists(p)))
        createFile();

	//
	// TODO
	// Call H5Fopen with the correct parameters
	//
	// Signature of the function : 
	// hid_t H5Fopen( const char *name, unsigned flags, hid_t fapl_id)
	//
	// Hint 1: To convert _filename to a char *, use _filename.c_str() 
	// Hint 2: You can use H5P_DEFAULT
	// Hint 3: The possible flags are H5F_ACC_TRUNC, H5F_ACC_EXCL, H5F_ACC_RDONLY, H5F_ACC_RDWR
	//
    if (_readOnly)
        _file_id = H5Fopen(...);
    else
        _file_id = H5Fopen(...);
    _is_closed = false;
}

void HDF5DataStore::closeFile()
{
    if (! _is_closed) {
		//
		// TODO
		//
		// Call H5Fclose with the right arguments
		//
		// Signature of the function : 
		// herr_t H5Fclose(hid_t file_id)
		//
        H5Fclose(...);
        _is_closed = true;
    }
}

void HDF5DataStore::set(const char * name, const double * arr, size_t total_size[2])
{
	const int NDIM=2;
	hid_t dset_id;
	hid_t filespace, memspace;
	hsize_t file_size[NDIM], mem_size[NDIM];


	//
	// TODO
	//
	// Initialize file_size and mem_size to the correct values
	//
    file_size[0] = ...; 	file_size[1] = ...;
    mem_size[0] = ...; 	mem_size[1] = ...;

	//
	// TODO
	//
	// Create the file space using H5Screate_simple.
	// Signature of the function:
	// hid_t H5Screate_simple(int rank, const hsize_t * current_dims, const hsize_t * maximum_dims )
	// Hint 1: set maximum_dims to NULL for now.
	//
    filespace = H5Screate_simple(...);

	//
	// TODO
	//
	// Create the dataset in the file
	// Signature of the function:
	// hid_t H5Dcreate(hid_t loc_id, const char *name, hid_t dtype_id, hid_t space_id, hid_t lcpl_id, hid_t dcpl_id, hid_t dapl_id );
	// Hint 1: The types for "dtype_id" are of the form H5T_NATIVE_...
	// Hint 2: You just created the space_id
	// Hint 3: The last parameters are property id, use H5P_DEFAULT
	//
    dset_id = H5Dcreate(...);
    
	//
	// TODO
	//
	// Create the memory space using H5Screate_simple.
	// Signature of the function:
	// hid_t H5Screate_simple(int rank, const hsize_t * current_dims, const hsize_t * maximum_dims )
	// Hint 1: set maximum_dims to NULL for now.
	//
    memspace = H5Screate_simple(...);

	//
	// TODO
	//
	// Write the data with H5Dwrite
	// Signature of the function:
	// herr_t H5Dwrite(hid_t dataset_id, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t xfer_plist_id, const void * buf );
	//
	_status = H5Dwrite(...);

	//
	// TODO
	//
	// Close resources
	H5Dclose(...);
    H5Sclose(...);
    H5Sclose(...);
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

	//
	// TODO
	//
	// Open the existing dataset with H5Dopen and retrieve its dataspace.
	// Signature of the function
	// hid_t H5Dopen(hid_t loc_id, const char *name, hid_t dapl_id )
	// hid_t H5Dget_space(hid_t dset_id);
	//
    dset_id = H5Dopen(...);
	filespace = H5Dget_space(...);
	
	// Create the memory space
    memspace = H5Screate_simple(NDIM, mem_size, NULL);

	//
	// TODO
	//
	// Read the dataset with H5Dread
	// herr_t H5Dread(hid_t dataset_id, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t xfer_plist_id, const void * buf );
	//
	_status = H5Dread(...);

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


