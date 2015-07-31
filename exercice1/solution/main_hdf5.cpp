#include "HDF5DataStore.h"
#include <vector>
#include <iostream>

using namespace std;

int main()
{
    size_t dim1 = 15;
    size_t dim2 = 4;
    // Initializes vectors
    vector<double> one_d = vector<double>(dim1);
    for (size_t i=0; i<dim1; i++)
        one_d[i] = i;

    vector<double> two_d = vector<double>(dim1 * dim2);
    for (size_t i=0; i<dim1; i++)
        for (size_t j=0; j<dim2; j++)
            two_d[i*dim2 + j] = i*dim2 + j;

    // Store the data on disk
    HDF5DataStore ds1("myfile.h5",false,true);
    ds1.set("oned",one_d.data(), dim1);
    size_t dims[2] = { dim1, dim2 };
    ds1.set("twod", two_d.data(), dims);
    ds1.close();

    // Reading back
    HDF5DataStore ds2("myfile.h5",true);
    vector<double> one_d_read = vector<double>(dim1);
    vector<double> two_d_read = vector<double>(dim1 * dim2);
    ds2.get("oned",one_d_read.data(), dim1);
    ds2.get("twod",two_d_read.data(), dims);
    ds2.close();

    // Displaying result
    cout << "Data read for one_d:" << endl;
    for (size_t i=0; i<dim1; i++)
        cout << one_d_read[i] << endl;

    cout << "Data read for two_d:" << endl;
    for (size_t i=0; i<dim1; i++) {
        for (size_t j=0; j<dim2; j++)
        {
            cout << two_d_read[i*dim2+j] << "\t";
        }
        cout << endl;
    }
}
