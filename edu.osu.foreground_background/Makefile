DARWIN_VERSION = 1.7.0
DARWIN_HOME=/home/behrooz/Software/machine_learning/darwin.1.7
DARWIN_LIB=-L${DARWIN_HOME}/bin ${DARWIN_HOME}/bin/libdrwnVision.so.${DARWIN_VERSION}  ${DARWIN_HOME}/bin/libdrwnML.so.${DARWIN_VERSION} ${DARWIN_HOME}/bin/libdrwnPGM.so.${DARWIN_VERSION} ${DARWIN_HOME}/bin/libdrwnIO.so.${DARWIN_VERSION}  ${DARWIN_HOME}/bin/libdrwnBase.so.${DARWIN_VERSION} -lvl
GIT_HOME=/home/behrooz/git
CXX = g++
CC = gcc
CFLAGS = -c  -g -ffloat-store -fexceptions -rdynamic -w -fPIC
CXXFLAGS = -c  -g -ffloat-store -fexceptions  -rdynamic -w -D__STDC_LIMIT_MACROS -fPIC
EIGEN_HOME=/usr/include/eigen3/

OUTPUTDIR = ./bin/
MKDIR = mkdir -p $(OUTPUTDIR)
SOURCE = ./src/


INCLUDES_CC = -I./include/ -I/usr/local/include/opencv -I/usr/local/include -I/usr/local/include/vlfeat 


INCLUDES_CXX = -I./include/ -I$(GIT_HOME)/bm/bm-common/include -I$(GIT_HOME)/bm/bm-vision/include   -I/usr/local/include -I/usr/local/include/opencv -I${DARWIN_HOME}/include -I${DARWIN_HOME}/external -I${DARWIN_HOME}/src/lib -I/usr/local/include -I/usr/local/include/vlfeat -I/usr/local/include  -I$(EIGEN_HOME) -I/home/behrooz/Software/development/matlab2012b/extern/include -I$(GIT_HOME)/bm/bm-deeplearning/include -I/usr/include/libxml2/



LIB_CXX = -L. -L../bm-common/bin -L$(GIT_HOME)/bm/bm-common/bin -L$(GIT_HOME)/bm/bm-vision/bin -L$(GIT_HOME)/bm/bm-deeplearning/bin  -L/usr/local/cuda/lib64 -L/usr/local/lib -lvl   -lbm-common -lbm-vision -lglog  -lmlpack  $(DARWIN_LIB)  -L/home/behrooz/Software/machine_learning/caffe-master/build/lib  -larmadillo
###############################################
SOURCES_CPP = \
	$(SOURCE)*.cpp 
###############################################
all:build

TARGET=hudl
OBJS_CXX=
build: 
	$(MKDIR) 
	$(CXX) $(CXXFLAGS) $(INCLUDES_CXX)  $(LIB_CXX) $(SOURCES_CPP)
	$(CXX) *.o $(LIBS) $(LIB_CXX) `pkg-config --cflags --libs opencv`  -o$(OUTPUTDIR)$(TARGET)
	rm -rf ./*.o

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf ./*.o

