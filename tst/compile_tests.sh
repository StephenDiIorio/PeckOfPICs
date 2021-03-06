#!/bin/bash

# export DEPS='DataStorage_1D.o DataStorage_2D.o'

export TDEPS='../obj/DataStorage_1D.o ../obj/DataStorage_2D.o'
export TDEPS=${TDEPS}' ../obj/DataStorage.o ../obj/FFT.o ../obj/Field.o'
export TDEPS=${TDEPS}' ../obj/FileIO.o ../obj/GridObject.o ../obj/Particle.o'
export TDEPS=${TDEPS}' ../obj/Species.o ../obj/ThreeVec.o'
# for filename in ../obj/*; do export DEPS=${DEPS}' '$filename; done

export H5_ROOT=$(brew --prefix hdf5)
export SZIP_ROOT=$(brew --prefix szip)

export H5_COMPILEFLAGS='-I'${H5_ROOT}'/include'
export H5_LINKFLAGS='-L'${H5_ROOT}'/lib -lhdf5_cpp -lhdf5 -lz -lm '
export H5_COMPILEFLAGS=${H5_COMPILEFLAGS}'-L'${SZIP_ROOT}'/lib -lsz'

export INCLUDE=${H5_COMPILEFLAGS}
export LDLIBS=${H5_LINKFLAGS}

echo $TDEPS
echo $INCLUDE
echo $LDLIBS

g++ -std=c++11 -g test_field_2.cpp -o bin/test_field_2.exe $TDEPS $LDLIBS
g++ -std=c++11 -g test_field_to_particle.cpp -o bin/test_field_to_particle.exe $TDEPS $LDLIBS
g++ -std=c++11 -g test_push.cpp -o bin/test_push.exe $TDEPS $LDLIBS