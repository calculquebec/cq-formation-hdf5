#include "HDF5DataStore.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t dim1 = 7;
    size_t dim2 = 4;
    // Initializes vectors
    vector<double> one_d = vector<double>(dim1);
    for (size_t i=0; i<dim1; i++)
        one_d[i] = rank;

    vector<double> two_d = vector<double>(dim1 * dim2);
    for (size_t i=0; i<dim1; i++)
        for (size_t j=0; j<dim2; j++)
            two_d[i*dim2 + j] = rank;

    // Store the data on disk
    HDF5DataStore ds1("myfile.h5",false,true);
    ds1.createGroup("/my_group");

    if (rank == 0) ds1.set("/my_group/oned",one_d.data(), 0, 1, dim1);
    if (rank == 1) ds1.set("/my_group/oned",one_d.data(), 1, 0, dim1);
    if (rank == 2) ds1.set("/my_group/oned",one_d.data(), 1, 4, dim1);
    if (rank == 3) ds1.set("/my_group/oned",one_d.data(), 5, 2, dim1);

    size_t total_size[2] = { dim1, dim2 };
    size_t start[2];
    size_t local_size[2];
    if (rank == 0) { start[0] = 0; start[1] = 0; local_size[0] = 3; local_size[1] = 2; }
    if (rank == 1) { start[0] = 0; start[1] = 2; local_size[0] = 3; local_size[1] = 2; }
    if (rank == 2) { start[0] = 3; start[1] = 0; local_size[0] = 4; local_size[1] = 1; }
    if (rank == 3) { start[0] = 3; start[1] = 1; local_size[0] = 4; local_size[1] = 3; }
    ds1.set("twod", two_d.data(), start, local_size, total_size);

    string attr_name = "my_attribute";
    string attr_val = "my_value";
    ds1.setAttribute(attr_name, attr_val, "/my_group/oned");
    ds1.close();

    // Reading back
    HDF5DataStore ds2("myfile.h5",true);
    vector<double> one_d_read = vector<double>(dim1);
    vector<double> two_d_read = vector<double>(dim1 * dim2);
    if (rank == 0) {
        ds2.get("/my_group/oned",one_d_read.data(), 0, dim1, dim1);
        ds2.get("twod",two_d_read.data(), start, total_size, total_size);

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
        string attr_val_read = ds2.getAttribute(attr_name,"/my_group/oned");
        cout << "Value of attribute " << attr_name << " is " << attr_val_read << endl;
    }
    ds2.close();

    MPI_Finalize();
}
