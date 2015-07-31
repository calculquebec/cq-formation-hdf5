# ############################
# LIST OF OBJECTS/EXES
# ############################
EXES = main_hdf5.out
EXE_EXTENSION = .out
OBJECTS = HDF5DataStore.o

CPP = mpic++
CPPFLAGS = -O2 --std=c++11 
GLOBAL_DEPEND = Makefile
LDFLAGS = -lboost_filesystem -lboost_system -lhdf5 -lhdf5_hl

all:: $(EXES)
clean:
	rm -rf $(EXES) $(OBJECTS) myfile.h5

main_hdf5.out: main_hdf5.cpp HDF5DataStore.o HDF5DataStore.h $(GLOBAL_DEPEND)
	$(CPP) $(CPPFLAGS) HDF5DataStore.o main_hdf5.cpp -o $*.out $(LDFLAGS)

%.o: %.cpp $(GLOBAL_DEPEND)
	$(CPP) $(CPPFLAGS) -c $*.cpp -o $*.o 

%.out: %.cpp $(GLOBAL_DEPEND)
	$(CC) $(CFLAGS) $*.cpp -o $*.out

