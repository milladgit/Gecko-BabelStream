
GECKO_DIR = /home/mghane/gecko

COMPILER = PGI
TARGET = ALL

ifndef COMPILER
define compiler_help
Set COMPILER to ensure correct flags are set.
Available compilers are:
  PGI CRAY
endef
$(info $(compiler_help))
endif

COMPILER_ = $(CXX)
COMPILER_PGI = pgc++
COMPILER_CRAY = CC

FLAGS_ = -O3 -std=c++11

# FLAGS_PGI = -std=c++11 -w -mp -acc
FLAGS_PGI = -m64 -std=c++11 -w -Mllvm -mp -acc
ifeq ($(COMPILER), PGI)
define target_help
Set a TARGET to ensure PGI targets the correct offload device.
Available targets are:
  SNB, IVB, HSW
  KEPLER, MAXWELL, PASCAL, VOLTA
  HAWAII
endef
ifndef TARGET
$(error $(target_help))
endif
TARGET_FLAGS_SNB     = -ta=multicore -tp=sandybridge
TARGET_FLAGS_IVB     = -ta=multicore -tp=ivybridge
TARGET_FLAGS_HSW     = -ta=multicore -tp=haswell
TARGET_FLAGS_KEPLER  = -ta=nvidia:cc35
TARGET_FLAGS_MAXWELL = -ta=nvidia:cc50
TARGET_FLAGS_PASCAL  = -ta=nvidia:cc60
TARGET_FLAGS_VOLTA   = -ta=nvidia:cc70
TARGET_FLAGS_HAWAII  = -ta=radeon:hawaii
TARGET_FLAGS_ALL 	 = -ta=multicore,tesla:cc70 -Minfo=accel
ifeq ($(TARGET_FLAGS_$(TARGET)),)
$(error $(target_help))
endif

FLAGS_PGI += $(TARGET_FLAGS_$(TARGET))

FLAGS_PGI += -DCUDA_ENABLED -I$(CUDA_HOME)/include/ -I$(GECKO_DIR)/ -L$(GECKO_DIR)/lib -L$(CUDA_HOME)/lib64
FLAGS_LIB = $(GECKO_DIR)/lib/libgecko.a -lcudart 

endif

FLAGS_CRAY = -hstd=c++11
CXXFLAGS = $(FLAGS_$(COMPILER))

gecko-stream: main.cpp GeckoStream.cpp
	$(COMPILER_$(COMPILER)) $(CXXFLAGS) -DGECKO main.cpp GeckoStream.cpp $(EXTRA_FLAGS) -o $@ $(FLAGS_LIB)

.PHONY: clean
clean:
	rm -f gecko-stream main.o GeckoStream.o


