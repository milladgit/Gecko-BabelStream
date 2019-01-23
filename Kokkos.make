
default: kokkos-stream

KOKKOS_PATH = /home/millad/github/kokkos
KOKKOS_DEVICES = OpenMP


vpath %.cpp $(sort $(dir $(SRC)))


ifndef COMPILER
define compiler_help
Set COMPILER to change flags (defaulting to GNU).
Available compilers are:
  GNU INTEL

endef
$(info $(compiler_help))
COMPILER=GNU
endif

COMPILER_GNU = g++
COMPILER_INTEL = icpc -qopt-streaming-stores=always
CXX_FINAL = $(COMPILER_$(COMPILER))

ifndef TARGET
define target_help
Set TARGET to change to offload device. Defaulting to CPU.
Available targets are:
  CPU (default)
  GPU
endef
$(info $(target_help))
TARGET=CPU
endif

# ifeq ($(TARGET), GPU)
# CXX = $(NVCC_WRAPPER)
# endif


# ifneq (,$(findstring Cuda,$(KOKKOS_DEVICES)))
ifeq ($(TARGET), GPU)
CXX = ${KOKKOS_PATH}/bin/nvcc_wrapper
CXXFLAGS = 
LDFLAGS = 
KOKKOS_DEVICES = "Cuda,OpenMP"
KOKKOS_ARCH = "HSW,Volta70"
KOKKOS_CUDA_OPTIONS += "enable_lambda"
else
CXX = g++
CXXFLAGS = -O3
LDFLAGS =  
KOKKOS_DEVICES = "OpenMP"
KOKKOS_ARCH = "HSW"
endif




# OBJ = main.o KokkosStream.o $(notdir $(SRC:.cpp=.o))
OBJ = main.o KokkosStream.o $(notdir $(SRC:.cpp=.o))
LIB = 

include $(KOKKOS_PATH)/Makefile.kokkos
#include $(KOKKOS_PATH)/Makefile

# kokkos-stream: $(OBJ) 
# 	$(CXX) $(KOKKOS_LDFLAGS) -DKOKKOS -O3 $(EXTRA_FLAGS) $(OBJ) -o $@ $(KOKKOS_LIBS) 

# %.o: %.cpp $(KOKKOS_CPP_DEPENDS)
# 	$(CXX) $(KOKKOS_CPPFLAGS) $(KOKKOS_CXXFLAGS) -DKOKKOS -O3 $(EXTRA_FLAGS) -c $<


kokkos-stream: $(OBJ) $(KOKKOS_LINK_DEPENDS)
	$(CXX) $(KOKKOS_LDFLAGS) $(LDFLAGS) -O3 $(EXTRA_PATH) $(OBJ) $(KOKKOS_LIBS) $(LIB) -o $@

%.o:%.cpp $(KOKKOS_CPP_DEPENDS)
	$(CXX) $(KOKKOS_CPPFLAGS) $(KOKKOS_CXXFLAGS) -DKOKKOS -O3 $(CXXFLAGS) $(EXTRA_INC) -c $< -o $(notdir $@)


.PHONY: clean
clean:
	rm -f kokkos-stream main.o KokkosStream.o *.o 

