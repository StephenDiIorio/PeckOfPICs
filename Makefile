RM=rm -rf
CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic -O3
LDFLAGS=-g -O3

H5_ROOT = $(shell brew --prefix hdf5)
SZIP_ROOT = $(shell brew --prefix szip)

H5_COMPILEFLAGS = -I$(H5_ROOT)/include
H5_LINKFLAGS    = -L$(H5_ROOT)/lib -lhdf5_cpp -lhdf5 -lz -lm \
                  -L$(SZIP_ROOT)/lib -lsz

INCLUDE=$(H5_COMPILEFLAGS)
LDLIBS=$(H5_LINKFLAGS)

TARGET=pic

all: main

SRCDIR=src
BINDIR=bin
OBJDIR=obj

SRCFILES=pic.cpp Simulation.cpp Particle.cpp Species.cpp Field.cpp FFT.cpp ThreeVec.cpp GridObject.cpp DataStorage.cpp DataStorage_1D.cpp DataStorage_2D.cpp FileIO.cpp

OBJFILES:=$(SRCFILES:.cpp=.o)

FULLTARGET=$(BINDIR)/$(TARGET)

# Path to look for source files
VPATH=$(SRCDIR):$(OBJDIR)

main: $(FULLTARGET)


# Rule to build the cpp files
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $(OBJDIR)/$@ $< $(INCLUDE)

$(FULLTARGET): $(OBJFILES)
	mkdir -p $(BINDIR)
	$(CXX) -o $@ $(addprefix $(OBJDIR)/,$(OBJFILES)) $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(OBJDIR)

cleanall: clean
	$(RM) $(BINDIR)

$(OBJFILES): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean cleanall main


# All the dependencies
pic.o: pic.cpp pic_tests.h two_stream.h cold_wave.h Simulation.h Species.h Particle.h ThreeVec.h Field.h FFT.h GridObject.h FileIO.h DataStorage.h DataStorage_1D.h DataStorage_2D.h
	$(CXX) -c $(CXXFLAGS) -o $(OBJDIR)/$@ $< $(INCLUDE)

Simulation.o: Simulation.cpp Simulation.h Species.h Particle.h ThreeVec.h Field.h FFT.h GridObject.h DataStorage_1D.h DataStorage_2D.h DataStorage.h
Particle.o: Particle.cpp Particle.h ThreeVec.h
Species.o: Species.cpp Species.h Particle.h ThreeVec.h Field.h FFT.h GridObject.h DataStorage_2D.h DataStorage.h DataStorage_1D.h
Field.o: Field.cpp Field.h FFT.h GridObject.h DataStorage_2D.h DataStorage.h
FFT.o: FFT.cpp FFT.h
ThreeVec.o: ThreeVec.cpp ThreeVec.h
GridObject.o: GridObject.cpp GridObject.h DataStorage_2D.h DataStorage.h
DataStorage.o: DataStorage.cpp DataStorage.h
DataStorage_1D.o: DataStorage_1D.cpp DataStorage.h DataStorage_1D.h
DataStorage_2D.o: DataStorage_2D.cpp DataStorage.h DataStorage_2D.h
FileIO.o: FileIO.cpp FileIO.h DataStorage.h
