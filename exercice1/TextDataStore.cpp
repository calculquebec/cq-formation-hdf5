#include "TextDataStore.h"
#include <iomanip>

#include <exception>
#include <boost/filesystem.hpp>

using namespace std;

TextDataStore::TextDataStore(const string & in_filename, const bool readOnly, const bool truncate)
    : _readOnly(readOnly), _truncate(truncate), _filename(in_filename)
{
    openFile();
}

TextDataStore::~TextDataStore()
{
    closeFile();
}

void TextDataStore::createFile()
{
    _is_closed = false;
    if (_truncate)
        _myfile.open(_filename, ios::out | ios::trunc);
    else
        _myfile.open(_filename, ios::out);
    _myfile.close();
}

void TextDataStore::openFile()
{
    boost::filesystem::path p(_filename);
    // if the store is not in read only, and either truncate is true, or the file does not exist, we create it
    if (! _readOnly && (_truncate || !boost::filesystem::exists(p)))
        createFile();

    if (_readOnly)
        _myfile.open(_filename, ios::in);
    else
        _myfile.open(_filename, ios::out);
    _myfile << setprecision(16) << fixed;
    _is_closed = false;
}

void TextDataStore::closeFile()
{
    if (! _is_closed) {
        _myfile.close();
        _is_closed = true;
    }
}

void TextDataStore::set(const double * arr, size_t total_size[2])
{
    size_t dimx = total_size[0];
    size_t dimy = total_size[1];
    for (size_t i=0; i<dimx; i++)
    {
        for (size_t j=0; j<dimy; j++)
        {
            _myfile << arr[i*dimy + j] << "\t";
        }
        _myfile << "\n";
    }
}
void TextDataStore::get(double * arr, size_t total_size[2])
{
    size_t dimx = total_size[0];
    size_t dimy = total_size[1];
    for (size_t i=0; i<dimx; i++)
    {
        for (size_t j=0; j<dimy; j++)
        {
            _myfile >> arr[i*dimy + j];
        }
    }
}
void TextDataStore::set(const double * arr, size_t total_size)
{
    for (size_t i=0; i<total_size; i++)
        _myfile << arr[i] << "\n";
}
void TextDataStore::get(double * arr, size_t total_size)
{
    for (size_t i=0; i<total_size; i++)
        _myfile >> arr[i];
}


