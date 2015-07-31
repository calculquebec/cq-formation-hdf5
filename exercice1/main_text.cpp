#include "TextDataStore.h"
#include <vector>

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
    TextDataStore ds1("oned.txt",false,true);
    ds1.set(one_d.data(), dim1);
    ds1.close();

    TextDataStore ds2("twod.txt",false,true);
    size_t dims[2] = { dim1, dim2 };
    ds2.set(two_d.data(), dims);
    ds2.close();


    // Reading back
    TextDataStore ds3("oned.txt",true);
    TextDataStore ds4("twod.txt",true);
    vector<double> one_d_read = vector<double>(dim1);
    vector<double> two_d_read = vector<double>(dim1 * dim2);
    ds3.get(one_d_read.data(), dim1);
    ds4.get(two_d_read.data(), dims);

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
