#ifndef HDF5DataStore_h
#define HDF5DataStore_h
#include "hdf5.h"
#include <string>
#include "hdf5_hl.h"

class HDF5DataStore
{
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

		/** Writes a 2D dataset in the file.
		 * @param name Name of the dataset
		 * @param arr The array to be written. It is assumed to be contiguous in memory.
		 * @param total_size Total dimension of the grid being written
		 **/
		void set(const char * name, const double * arr, size_t total_size[2]);

		/** Retreives a 2D dataset from the file.
		 * @param name Name of the dataset
		 * @param arr The array to be read. It is assumed to be contiguous in memory.
		 * @param total_size Total dimension of the grid being read.
		 **/
		void get(const char * name, double * arr, size_t total_size[2]);

		/** Writes a 1D dataset in the file.
		 * @param name Name of the dataset
		 * @param arr The array to be written. It is assumed to be contiguous in memory.
		 * @param total_size Total dimension of the dataset being written
		 **/
		void set(const char * name, const double * arr, size_t total_size);

		/** Retreives a 1D dataset in the file.
		 * @param name Name of the dataset
		 * @param arr The array to be read. It is assumed to be contiguous in memory.
		 * @param total_size Total dimension of the dataset being read.
		 **/
		void get(const char * name, double * arr, size_t total_size);

		/** Closes the Datastore */
		void close() { closeFile(); }
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

