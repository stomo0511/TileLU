#
BLAS_ROOT = /opt/OpenBLAS
BLAS_INC_DIR = $(BLAS_ROOT)/include
BLAS_LIB_DIR = $(BLAS_ROOT)/lib
BLAS_LIBS = -lopenblas
SBLAS_LIBS = -lopenblas_seq
#
LAPACK_ROOT = /opt/LAPACK_3.6.0
LAPACK_INC_DIR = $(LAPACK_ROOT)/include
LAPACK_LIB_DIR = $(LAPACK_ROOT)/lib
LAPACK_LIBS = -llapacke -llapack
#
PLASMA_ROOT = /opt/PLASMA
PLASMA_INC_DIR = $(PLASMA_ROOT)/include
PLASMA_LIB_DIR = $(PLASMA_ROOT)/lib
PLASMA_LIBS = -lplasma -lcoreblas -lquark 
#
TMATRIX_ROOT = /Users/stomo/WorkSpace_New/TileAlgorithm/TileMatrix
TMATRIX_INC_DIR = $(TMATRIX_ROOT)
TMATRIX_LIB_DIR = $(TMATRIX_ROOT)
TMATRIX_LIBS = -lTileMatrix
#
COREBLAS_ROOT = /Users/stomo/WorkSpace_New/TileAlgorithm/CoreBlas
COREBLAS_INC_DIR = $(COREBLAS_ROOT)
COREBLAS_LIB_DIR = $(COREBLAS_ROOT)
COREBLAS_LIBS = -lCoreBlasTile
#
CXX =	/usr/local/bin/g++ -fopenmp
# for DEBUG
CXXFLAGS =	-DDEBUG -g -I$(TMATRIX_INC_DIR) -I$(PLASMA_INC_DIR) -I$(TMATRIX_INC_DIR) -I$(COREBLAS_INC_DIR)
# for Performance evaluation
#CXXFLAGS =	-O2 -I$(BLAS_INC_DIR) -I$(PLASMA_INC_DIR) -I$(TMATRIX_INC_DIR) -I$(COREBLAS_INC_DIR)

RLOBJS = TileLU.o RightLooking_incpiv.o
RTOBJS = TileLU.o RightLooking_incpiv_Task.o
PROBJS = TileLU.o Panel_RL_Task.o

all:	RL RT

PR:	$(PROBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(PROBJS) \
				-L$(TMATRIX_LIB_DIR) $(TMATRIX_LIBS) \
				-L$(COREBLAS_LIB_DIR) $(COREBLAS_LIBS) \
				-L$(PLASMA_LIB_DIR) $(PLASMA_LIBS) \
				-L$(BLAS_LIB_DIR) $(SBLAS_LIBS)

RL:	$(RLOBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(RLOBJS) \
				-L$(TMATRIX_LIB_DIR) $(TMATRIX_LIBS) \
				-L$(COREBLAS_LIB_DIR) $(COREBLAS_LIBS) \
				-L$(PLASMA_LIB_DIR) $(PLASMA_LIBS) \
				-L$(BLAS_LIB_DIR) $(SBLAS_LIBS)

RT:	$(RTOBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(RTOBJS) \
				-L$(TMATRIX_LIB_DIR) $(TMATRIX_LIBS) \
				-L$(COREBLAS_LIB_DIR) $(COREBLAS_LIBS) \
				-L$(PLASMA_LIB_DIR) $(PLASMA_LIBS) \
				-L$(BLAS_LIB_DIR) $(SBLAS_LIBS)

clean:
	rm -f PR $(PROBJS) RT $(RTOBJS) RL $(RLOBJS)
