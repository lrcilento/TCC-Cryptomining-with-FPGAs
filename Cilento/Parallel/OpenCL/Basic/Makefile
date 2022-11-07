ifeq ($(VERBOSE),1)
ECHO := 
else
ECHO := @
endif

# Where is the Intel(R) FPGA SDK for OpenCL(TM) software?
ifeq ($(wildcard $(INTELFPGAOCLSDKROOT)),)
$(error Set INTELFPGAOCLSDKROOT to the root directory of the Intel(R) FPGA SDK for OpenCL(TM) software installation)
endif
ifeq ($(wildcard $(INTELFPGAOCLSDKROOT)/host/include/CL/opencl.h),)
$(error Set INTELFPGAOCLSDKROOT to the root directory of the Intel(R) FPGA SDK for OpenCL(TM) software installation.)
endif

# OpenCL compile and link flags.
AOC_KERNEL          := aoc
AOCL_COMPILE_CONFIG := $(shell aocl compile-config )
AOCL_LINK_LIBS      := $(shell aocl ldlibs )
AOCL_LINK_FLAGS     := $(shell aocl ldflags )

# Linking with defences enabled
AOCL_LINK_FLAGS  += -z noexecstack
AOCL_LINK_FLAGS  += -Wl,-z,relro,-z,now
AOCL_LINK_FLAGS  += -Wl,-Bsymbolic
AOCL_LINK_FLAGS  += -pie
AOCL_LINK_CONFIG := $(AOCL_LINK_FLAGS) $(AOCL_LINK_LIBS)

# Compilation flags
ifeq ($(DEBUG),1)
CXXFLAGS += -g
else
CXXFLAGS += -O2
endif

ifeq ($(EMULATOR),1)
CXXFLAGS += -DEMULATOR
endif

ifeq ($(MODEL),1)
CXXFLAGS += -DMODEL1
else
CXXFLAGS += -DMODEL2
endif

# Compiling with defences enabled
CXXFLAGS += -fstack-protector
CXXFLAGS += -D_FORTIFY_SOURCE=2
CXXFLAGS += -Wformat -Wformat-security
CXXFLAGS += -fPIE

# We must force GCC to never assume that it can shove in its own
# sse2/sse3 versions of strlen and strcmp because they will CRASH.
# Very hard to debug!
CXXFLAGS += -fPIC

# Compiler
CXX := g++

ifeq ($(MODEL),1)
OCLFLAGS += -DMODEL1
else 
OCLFLAGS += -DMODEL2
endif

# Emulator
AOC_EMU  := $(AOC_KERNEL)
EMU      := -march=emulator
EMU_TYPE := -legacy-emulator
AOC_EMU  += $(EMU)
CONTEXT  := CL_CONTEXT_EMULATOR_DEVICE_INTELFPGA=1

# Early image
AOC_RTL := $(AOC_KERNEL)
RTL     := -rtl
AOC_RTL += $(RTL)

# FPGA image
AOC_FPGA := $(AOC_KERNEL)

# Target
TARGET_HST  := host
TARGET_DIR  := bin
TARGET_EMU  := emu
TARGET_RTL  := rtl
TARGET_BTS  := fpga
TARGET_SIGN := sign
TARGET_CNFG := config
RUN_EMU     := run-emu
RUN_FPGA    := run-fpga
RUN_FPGA0   := run-acl0
RUN_FPGA1   := run-acl1

# Directories
INC_DIRS := host/inc ../common/inc
LIB_DIRS := 
KER_DIR  := device/
CUR_DIR  := $(shell pwd)

# Files
INCS        := $(wildcard host/inc/*.h)
SRCS        := $(wildcard host/src/*.cpp ../common/src/AOCLUtils/*.cpp)
LIBS        := rt pthread
KERNEL      := $(wildcard device/*.cl)
PARAM       := host/inc/parameters.h
K_NAME      := $(KERNEL:device/%.cl=%)
AOCX        := $(KERNEL:device/%.cl=%.aocx)
SIGNED_AOCX := $(KERNEL:device/%.cl=%_signed.aocx)

# Sign OpenCL kernel
SIGN_CMD := ./sign_aocx.sh -H openssl_manager -r NULL -k NULL -i
CUR_DIR  := $(shell pwd)
UNSIGNED := $(CUR_DIR)/$(TARGET_DIR)/$(AOCX)
SIGNED   := $(CUR_DIR)/$(TARGET_DIR)/$(SIGNED_AOCX)

# Make it all!
all : $(TARGET_HST) $(TARGET_DIR) $(TARGET_EMU) $(TARGET_RTL) $(TARGET_BTS)

# Host executable target.
$(TARGET_HST) : Makefile $(SRCS) $(INCS) $(TARGET_DIR)
	$(ECHO)$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(EXTRACXXFLAGS) -fPIC $(foreach D,$(INC_DIRS),-I$D) \
			$(AOCL_COMPILE_CONFIG) $(SRCS) $(AOCL_LINK_CONFIG) \
			$(foreach D,$(LIB_DIRS),-L$D) \
			$(foreach L,$(LIBS),-l$L) \
			-o $(TARGET_DIR)/$(TARGET_HST)

# Create bin directory
$(TARGET_DIR) :
	$(ECHO)mkdir $(TARGET_DIR)

$(TARGET_CNFG) : $(PARAM)
	$(ECHO)sed -i "s/#define NZ.*/#define NZ $(nz)/" $(PARAM) && \
			sed -i "s/#define NX.*/#define NX $(nx)/" $(PARAM) && \
			sed -i "s/#define IT.*/#define IT $(iter)/" $(PARAM) && \
			sed -i "s/#define SPACE_ORDER.*/#define SPACE_ORDER $(order)/" $(PARAM)

# Emulation
$(TARGET_EMU) : $(KERNEL)
	$(ECHO)$(AOC_EMU) $(KERNEL) -o $(TARGET_DIR)/$(AOCX) $(EMU_TYPE) -DEMULATOR

# Early image with report
$(TARGET_RTL) : $(KERNEL)
	$(ECHO)cd $(TARGET_DIR) && $(AOC_RTL) $(OCLFLAGS) ../$(KERNEL) && cd ..

# FPGA image
$(TARGET_BTS) : $(KERNEL)
	$(ECHO)$(AOC_FPGA) $(KERNEL) -o $(TARGET_DIR)/$(AOCX) $(OCLFLAGS)

# Sign FPGA image
$(TARGET_SIGN) :
	$(ECHO)$(SIGN_CMD) $(UNSIGNED) -o $(SIGNED)

# Run Emulator
$(RUN_EMU) : $(TARGET_DIR)/$(AOCX)
	$(ECHO)$(CONTEXT) $(TARGET_DIR)/$(TARGET_HST)

# Run on device
$(RUN_FPGA) :
	$(ECHO)$(CUR_DIR)/$(TARGET_DIR)/$(TARGET_HST) && \
			rm -f $(SIGNED) && \
			rm -f $(CUR_DIR)/$(TARGET_DIR)/*.gbs*

# Run on device 0
$(RUN_FPGA0) : 
	$(ECHO)$(SIGN_CMD) $(UNSIGNED) -o $(SIGNED) && \
			aocl program acl0 $(SIGNED) && \
			rm -f $(SIGNED) && \
			rm -f $(CUR_DIR)/$(TARGET_DIR)/*.gbs* && \
			$(CUR_DIR)/$(TARGET_DIR)/$(TARGET_HST)

# Run on device 1
$(RUN_FPGA1) : 
	$(ECHO)$(SIGN_CMD) $(UNSIGNED) -o $(SIGNED) && \
			aocl program acl1 $(SIGNED) && \
			rm -f $(SIGNED) && \
			rm -f $(CUR_DIR)/$(TARGET_DIR)/*.gbs* && \
			$(CUR_DIR)/$(TARGET_DIR)/$(TARGET_HST)

# Clean targets
clean :
	$(ECHO)rm -rf $(TARGET_DIR)

.PHONY : all clean