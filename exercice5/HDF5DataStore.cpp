#include "HDF5DataStore.h"

#include <exception>
#include <boost/filesystem.hpp>

using namespace std;

size_t HDF5DataStore::start2d[2] = {0,0};

void HDF5DataStore::createMPIInfo()
{
	int status = MPI_Info_create(&_info);
}

void HDF5DataStore::freeMPIInfo()
{
	MPI_Info_free(&_info);
}

void HDF5DataStore::createAccessPList()
{
	_fapl = H5Pcreate(H5P_FILE_ACCESS);
	H5Pset_fapl_mpio(_fapl, MPI_COMM_WORLD, _info);

}
void HDF5DataStore::closeAccessPList()
{
	H5Pclose(_fapl);
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
    if (_truncate)
        _file_id = H5Fcreate(_filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, _fapl);
    else
        _file_id = H5Fcreate(_filename.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, _fapl);
    closeFile();
}

void HDF5DataStore::openFile()
{
    boost::filesystem::path p(_filename);
    if (! _readOnly && (_truncate || !boost::filesystem::exists(p)))
        createFile();
    if (_readOnly)
        _file_id = H5Fopen(_filename.c_str(), H5F_ACC_RDONLY, _fapl);
    else
        _file_id = H5Fopen(_filename.c_str(), H5F_ACC_RDWR, _fapl);
    _is_closed = false;
}

void HDF5DataStore::closeFile()
{
    if (! _is_closed) {
        H5Fclose(_file_id);
        _is_closed = true;
    }
}

void HDF5DataStore::set(const char * name, const double * arr, size_t start[2], size_t local_size[2], size_t total_size[2])
{
	const int NDIM=2;
	hid_t dset_id;
	hid_t filespace, memspace;
	hsize_t file_size[NDIM], mem_size[NDIM];
	hsize_t file_count[NDIM], mem_count[NDIM];
	hsize_t file_offset[NDIM], mem_offset[NDIM];

	// Set all the correct dimensions
	//
	// TODO
	//
	// Set the correct dimensions. Assume that the memory layout compact, starting at element
	// 0 of the array.
	//
	file_offset[0] = ; 		file_offset[1] = ;
	file_count[0] = ; 	file_count[1] = ;
    file_size[0] = ; 	file_size[1] = ;
	mem_offset[0] = ;				mem_offset[1] = ;
	mem_count[0] = ;	mem_count[1] = ;
	mem_size[0] = ;	mem_size[1] = ;

	// Create the filespace and dataset
    filespace = H5Screate_simple(NDIM, file_size, NULL); 
    dset_id = H5Dcreate(_file_id, name, H5T_NATIVE_DOUBLE, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	//
	// TODO
	// Select a hyperslab in the filespace using the function H5Sselect_hyperslab
	// Signature of the function:
	// herr_t H5Sselect_hyperslab(hid_t space_id, H5S_seloper_t op, const hsize_t *start, 
	// 	  const hsize_t *stride, const hsize_t *count, const hsize_t *block )
	//
	// Note: Using "NULL" for the value of one of the parameters will default to {1,1}.
	//
	// Select a hyperslab of the filespace
    H5Sselect_hyperslab(...);

	// Create the memory space
    memspace = H5Screate_simple(NDIM, mem_size, NULL);
	//
	// TODO
	// Select a hyperslab in the memory space using the function H5Sselect_hyperslab
	// Signature of the function:
	// herr_t H5Sselect_hyperslab(hid_t space_id, H5S_seloper_t op, const hsize_t *start, 
	// 	  const hsize_t *stride, const hsize_t *count, const hsize_t *block )
	//
	// Note: Using "NULL" for the value of one of the parameters will default to {1,1}.
	//
	// Select a hyperslab of the memory space
    H5Sselect_hyperslab(...);

	// Write data
	_status = H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, memspace, filespace, H5P_DEFAULT, arr);

	// Close resources
	H5Dclose(dset_id);
    H5Sclose(filespace);
    H5Sclose(memspace);
}
void HDF5DataStore::get(const char * name, double * arr, size_t start[2], size_t local_size[2], size_t total_size[2])
{
	const int NDIM=2;
	hid_t dset_id;
	hid_t filespace, memspace;
	hsize_t file_size[NDIM], mem_size[NDIM];
	hsize_t file_count[NDIM], mem_count[NDIM];
	hsize_t file_offset[NDIM], mem_offset[NDIM];

	// Set all the correct dimensions
	file_offset[0] = start[0]; 		file_offset[1] = start[1];
	file_count[0] = local_size[0]; 	file_count[1] = local_size[1];
    file_size[0] = total_size[0]; 	file_size[1] = total_size[1];
	mem_offset[0] = 0;				mem_offset[1] = 0;
	mem_count[0] = local_size[0];	mem_count[1] = local_size[1];
	mem_size[0] = local_size[0];	mem_size[1] = local_size[1];

	// Open the dataset and retrieve the space
    dset_id = H5Dopen(_file_id, name, H5P_DEFAULT);
    filespace = H5Dget_space(dset_id);
	// Select the hyperslab in the filespace
	H5Sselect_hyperslab(filespace, H5S_SELECT_SET, file_offset, NULL, file_count, NULL);

	// Create the memory space
    memspace = H5Screate_simple(NDIM, mem_size, NULL);
    H5Sselect_hyperslab(memspace, H5S_SELECT_SET, mem_offset, NULL, mem_count, NULL);

	// Read the data
	_status = H5Dread(dset_id, H5T_NATIVE_DOUBLE, memspace, filespace, H5P_DEFAULT, arr);

	// Close resources.
	H5Dclose(dset_id);
    H5Sclose(filespace);
    H5Sclose(memspace);
}
void HDF5DataStore::set(const char * name, const double * arr, size_t start, size_t local_size, size_t total_size)
{
	const int NDIM=1;
	hid_t dset_id;
	hid_t filespace, memspace;
	hsize_t file_size[NDIM], mem_size[NDIM];
	hsize_t file_count[NDIM], mem_count[NDIM];
	hsize_t file_offset[NDIM], mem_offset[NDIM];

	// Set all the correct dimensions
	file_offset[0] = start;
	file_count[0] = local_size;
    file_size[0] = total_size;
	mem_offset[0] = 0;
	mem_count[0] = local_size;
	mem_size[0] = local_size;

	// Create the filespace and dataset
    filespace = H5Screate_simple(NDIM, file_size, NULL); 
    dset_id = H5Dcreate(_file_id, name, H5T_NATIVE_DOUBLE, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	// Select a hyperslab of the filespace
    H5Sselect_hyperslab(filespace, H5S_SELECT_SET, file_offset, NULL, file_count, NULL);

    // Create the memory space
    memspace = H5Screate_simple(NDIM, mem_size, NULL);
	// Select a hyperslab of the memory space
    H5Sselect_hyperslab(memspace, H5S_SELECT_SET, mem_offset, NULL, mem_count, NULL);

	// Write data
	_status = H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, memspace, filespace, H5P_DEFAULT, arr);

	// Close resources
	H5Dclose(dset_id);
    H5Sclose(filespace);
    H5Sclose(memspace);
}
void HDF5DataStore::get(const char * name, double * arr, size_t start, size_t local_size, size_t total_size)
{
	const int NDIM=1;
	hid_t dset_id;
	hid_t filespace, memspace;
	hsize_t file_size[NDIM], mem_size[NDIM];
	hsize_t file_count[NDIM], mem_count[NDIM];
	hsize_t file_offset[NDIM], mem_offset[NDIM];

	// Set all the correct dimensions
	file_offset[0] = start;
	file_count[0] = local_size;
    file_size[0] = total_size;
	mem_offset[0] = 0;
	mem_count[0] = local_size;
	mem_size[0] = local_size;

	// Open the dataset and retrieve the space
    dset_id = H5Dopen(_file_id, name, H5P_DEFAULT);
    filespace = H5Dget_space(dset_id);
	// Select the hyperslab in the filespace
	H5Sselect_hyperslab(filespace, H5S_SELECT_SET, file_offset, NULL, file_count, NULL);

	// Create the memory space
    memspace = H5Screate_simple(NDIM, mem_size, NULL);
    H5Sselect_hyperslab(memspace, H5S_SELECT_SET, mem_offset, NULL, mem_count, NULL);

	// Read the data
	_status = H5Dread(dset_id, H5T_NATIVE_DOUBLE, memspace, filespace, H5P_DEFAULT, arr);

	// Close resources.
	H5Dclose(dset_id);
    H5Sclose(filespace);
    H5Sclose(memspace);
}


