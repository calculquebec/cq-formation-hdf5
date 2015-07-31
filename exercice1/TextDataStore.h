#ifndef TextDataStore_h
#define TextDataStore_h
#include <string>
#include <iostream>
#include <fstream>

class TextDataStore
{
	public:
		/** Creates a Text data store to store or retreive informations from an Text file.
		 * @param in_filename The name of the file to be created or read
		 * @param readOnly True if the file is to be opened in read only mode.
		 * @param truncate True if the file is to be truncated if it exists
		 * @note The file will be created if it does not exist.
		 **/
		TextDataStore(const std::string & in_filename, const bool readOnly=false, const bool truncate=false);

		/** Destructs the datastore and ensures the file is close.
		 **/
		~TextDataStore();

		/** Writes a 2D dataset in the file.
		 * @param arr The array to be written.
		 * @param total_size Total dimension of the grid being written
		 * @note The array is assumed to be in a row-major format, with a total length of total_size[0]*total_size[1]
		 **/
		void set(const double * arr, size_t total_size[2]);

		/** Retreives a 2D dataset from the file.
		 * @param arr The array to be read. It is assumed to be contiguous in memory.
		 * @param total_size Total dimension of the grid being read.
		 * @note The array is assumed to be in a row-major format, with a total length of total_size[0]*total_size[1]
		 **/
		void get(double * arr, size_t total_size[2]);

		/** Writes a 1D dataset in the file.
		 * @param arr The array to be written. It is assumed to be contiguous in memory.
		 * @param total_size Total dimension of the dataset being written
		 **/
		void set(const double * arr, size_t total_size);

		/** Retreives a 1D dataset in the file.
		 * @param arr The array to be read. It is assumed to be contiguous in memory.
		 * @param total_size Total dimension of the dataset being read.
		 **/
		void get(double * arr, size_t total_size);

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
		std::fstream _myfile;
};


#endif

