#ifndef HDF5DataStore_h
#define HDF5DataStore_h
#include "hdf5.h"
#include <string>
#include "hdf5_hl.h"

class HDF5DataStore
{
	public:
		static size_t start2d[2];

	public:
		/** Creates a HDF5 data store to store or retreive informations from an HDF5 file.
		 * @param in_filename The name of the file to be created or read
		 * @param readOnly True if the file is to be opened in read only mode.
		 * @param truncate True if the file is to be truncated if it exists
		 * @note The file will be created if it does not exist.
		 **/
		HDF5DataStore(const std::string & in_filename, const bool readOnly=false, const bool truncate=false);

		/** Destructs the datastore and ensures the file is close.
		 **/
		~HDF5DataStore();

	private:
		void createMPIInfo();
		void freeMPIInfo();
		void createAccessPList();
		void closeAccessPList();
		MPI_Info _info;
		hid_t _fapl;

	public:
		/** Writes a 2D dataset in the file.
		 * @param name Name of the dataset
		 * @param arr The array to be written. It is assumed to be contiguous in memory.
		 * @param start The starting point for the array, in file
		 * @param local_size The size of the array, in file
		 * @param total_size Total dimension of the grid being written
		 **/
		void set(const char * name, const double * arr, size_t start[2], size_t local_size[2], size_t total_size[2]);
		void set(const char * name, const double * arr, size_t total_size[2]) { set(name, arr, total_size, start2d, total_size); }

		/** Retreives a 2D dataset from the file.
		 * @param name Name of the dataset
		 * @param arr The array to be read. It is assumed to be contiguous in memory.
		 * @param start The starting point for the array, in file
		 * @param local_size The size of the array, in file
		 * @param total_size Total dimension of the grid being read.
		 **/
		void get(const char * name, double * arr, size_t start[2], size_t local_size[2], size_t total_size[2]);
		void get(const char * name, double * arr, size_t total_size[2]) { get(name, arr, total_size, start2d, total_size); }

		/** Writes a 1D dataset in the file.
		 * @param name Name of the dataset
		 * @param arr The array to be written. It is assumed to be contiguous in memory.
		 * @param start The starting point for the array, in file
		 * @param local_size The size of the array, in file
		 * @param total_size Total dimension of the dataset being written
		 **/
		void set(const char * name, const double * arr, size_t start, size_t local_size, size_t total_size);
		void set(const char * name, const double * arr, size_t total_size) { set(name,arr,0,total_size,total_size); }

		/** Retreives a 1D dataset in the file.
		 * @param name Name of the dataset
		 * @param arr The array to be read. It is assumed to be contiguous in memory.
		 * @param start The starting point for the array, in file
		 * @param local_size The size of the array, in file
		 * @param total_size Total dimension of the dataset being read.
		 **/
		void get(const char * name, double * arr, size_t start, size_t local_size, size_t total_size);
		void get(const char * name, double * arr, size_t total_size) { get(name,arr,0,total_size,total_size); }
		
		/** Creates a group in the HDF5 file.
		 * @param name The name of the group.
		 **/
		void createGroup(const char * name);
		/** Creates a group in the HDF5 file.
		 * @param name The name of the group.
		 **/
		void createGroup(const std::string & name) { createGroup(name.c_str()); }
		
		/** Writes an attribute as an HDF5 attribute in the file, as a string.
		 * @param name Name of the attribute
		 * @param val Value of the attribute.
		 * @param dset_name Value of the dataset on which to set the attribute (by default, on "/"). 
		 **/
		void setAttribute(const char * name, const char * val, const char * dset_name="/");
		/** Writes an attribute as an HDF5 attribute in the file.
		 * @param name Name of the attribute
		 * @param val Value of the attribute.
		 * @param dset_name Value of the dataset on which to set the attribute (by default, on "/"). 
		 **/
		void setAttribute(const char * name, const std::string & val, const char * dset_name="/") { this->setAttribute(name, val.c_str(), dset_name); }
		/** Writes an attribute as an HDF5 attribute in the file.
		 * @param name Name of the attribute
		 * @param val Value of the attribute.
		 * @param dset_name Value of the dataset on which to set the attribute (by default, on "/"). 
		 **/
		void setAttribute(const std::string & name, const std::string & val, const std::string & dset_name="/") { this->setAttribute(name.c_str(), val.c_str(), dset_name.c_str()); }

		/** Reads an attribute from the HDF5 file, as a string.
		 * @param name Name of the attribute.
		 * @param dset_name Value of the dataset from which to get the attribute (by default, from "/"). 
		 * @return Value of the attribute.
		 **/
		std::string getAttribute(const char * name, const char * dset_name="/");
		/** Reads an attribute from the HDF5 file, as a string.
		 * @param name Name of the attribute.
		 * @param dset_name Value of the dataset from which to get the attribute (by default, from "/"). 
		 * @return Value of the attribute.
		 **/
		std::string getAttribute(const std::string & name, const std::string & dset_name="/") { return getAttribute(name.c_str(),dset_name.c_str()); }


		/** Closes the Datastore */
		void close();
	private:
		void createFile();
		void closeFile();
		void openFile();

		bool _is_closed;
		bool _readOnly;
		bool _truncate;
		std::string _filename;
		hid_t _file_id;
		herr_t _status;
};

#endif

