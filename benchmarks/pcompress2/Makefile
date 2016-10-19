ARCH = arm

ifeq ($(ARCH),x86)
        CXX = gcc
        CXXFLAGS = -DX86_ARCH
        EXTRAFLAGS = -fstack-usage
else ifeq ($(ARCH),alpha)
        #CXX = alpha-linux-gnu-gcc-4.7
        CXX = alphaev67-unknown-linux-gnu-gcc
        ISA_FILE = $(SPM_GEM5_DIR)/util/m5/m5op_alpha.S
        CXXFLAGS = -DALPHA_ARCH
        #EXTRAFLAGS = -fstack-usage
        INCLUDES = -I$(SPM_GEM5_DIR)/src/mem/spm/api -I$(SPM_GEM5_DIR)/util/m5 $(ISA_FILE)
else ifeq ($(ARCH),arm)
        # CXX = arm-linux-gnueabi-gcc
        # ISA_FILE = $(SPM_GEM5_DIR)/util/m5/m5op_arm.S
        # CXXFLAGS = -DARM32_ARCH
        CXX = aarch64-linux-gnu-gcc
        ISA_FILE = $(SPM_GEM5_DIR)/util/m5/m5op_arm_A64.S
        CXXFLAGS = -DARM64_ARCH
        EXTRAFLAGS = -fstack-usage
        INCLUDES = -I$(SPM_GEM5_DIR)/src/mem/spm/api -I$(SPM_GEM5_DIR)/util/m5 $(ISA_FILE)
endif

BINDIR = bin/$(ARCH)

DEBUG = 0
OLEVEL = 3
ifeq ($(DEBUG), 1)
	OPTFLAGS = -g -Wall
else
	OPTFLAGS = -O$(OLEVEL) -Wall
endif

LDFLAGS = -static

pcompress2:  pcompress2.c Makefile
	$(CXX) $(OPTFLAGS) $(LDFLAGS) $(CXXFLAGS) $(EXTRAFLAGS) $(INCLUDES) -o $(BINDIR)/pcompress2  arithmetic.c unarithmetic.c compress.c compress.h pcompress2.c spm_management.h uncompress.c uncompress.h

clean:
	rm -rf *.o $(BINDIR)/* output* *.su
