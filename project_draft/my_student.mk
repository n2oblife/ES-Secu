##### Student Makefile for the ESS deliverables #####
# Modify this file as needed

# Name of each project
LD_NAME_DIMMING = DIMMING
LD_NAME_PARTA = PartA
LD_NAME_PARTB = PartB

# Source directories for PartA and PartB
SRC_DIR_DIMMING = src/CCU_intro/ccu
SRC_DIR_PARTA = src/PartA
SRC_DIR_PARTB = src/PartB

# Define build output directories for each part
BUILD_DIR_DIMMING = build/dimming
BUILD_DIR_PARTA = build/PartA
BUILD_DIR_PARTB = build/PartB

# Gather all source files in src/PartA and src/PartB
SRCS_DIMMING = $(wildcard $(SRC_DIR_DIMMING)/*.c)
SRCS_PARTA = $(wildcard $(SRC_DIR_PARTA)/*.c)
SRCS_PARTB = $(wildcard $(SRC_DIR_PARTB)/*.c)

# Set linker output for PartA and PartB
LD_NAME = $(LD_NAME_DIMMING) $(LD_NAME_PARTA) $(LD_NAME_PARTB)

# Precompiled libraries, e.g., -lm for math functions
LIBLNK =

# Path to the XMC library (will be overwritten during grading)
XMC_LIBDIR = /opt/XMClib/XMC_Peripheral_Library_v2.1.16

# Language dialect and compiler flags
SCFLAGS  = -std=gnu99
SCFLAGS += -O0  # Set optimization level to 0 for easier debugging
SCFLAGS += -Wall  # Enable basic warnings

# Override OBJS variable to separate object files for each part
OBJS_DIMMING = $(patsubst %.c, $(BUILD_DIR_DIMMING)/%.o, $(notdir $(SRCS_DIMMING)))
OBJS_PARTA = $(patsubst %.c, $(BUILD_DIR_PARTA)/%.o, $(notdir $(SRCS_PARTA)))
OBJS_PARTB = $(patsubst %.c, $(BUILD_DIR_PARTB)/%.o, $(notdir $(SRCS_PARTB)))

# Targets for building each part
.PHONY: all DIMMING PartA PartB clean

all: DIMMING PartA PartB

DIMMING: $(BUILD_DIR_DIMMING)/$(LD_NAME_DIMMING).elf

PartA: $(BUILD_DIR_PARTA)/$(LD_NAME_PARTA).elf

PartB: $(BUILD_DIR_PARTB)/$(LD_NAME_PARTB).elf

# Rule to compile DIMMING
$(BUILD_DIR_DIMMING)/%.o: $(SRC_DIR_DIMMING)/%.c | $(BUILD_DIR_DIMMING)
	$(CC) $(CFLAGS) -o $@ $<

# Rule to compile PartA
$(BUILD_DIR_PARTA)/%.o: $(SRC_DIR_PARTA)/%.c | $(BUILD_DIR_PARTA)
	$(CC) $(CFLAGS) -o $@ $<

# Rule to compile PartB
$(BUILD_DIR_PARTB)/%.o: $(SRC_DIR_PARTB)/%.c | $(BUILD_DIR_PARTB)
	$(CC) $(CFLAGS) -o $@ $<

# Rule to link DIMMING
$(BUILD_DIR_DIMMING)/$(LD_NAME_DIMMING).elf: $(OBJS_DIMMING)
	$(LD) $(LFLAGS) -o $@ $(OBJS_DIMMING) $(LIBLNK)
	$(SZ) $@

# Rule to link PartA
$(BUILD_DIR_PARTA)/$(LD_NAME_PARTA).elf: $(OBJS_PARTA)
	$(LD) $(LFLAGS) -o $@ $(OBJS_PARTA) $(LIBLNK)
	$(SZ) $@

# Rule to link PartB
$(BUILD_DIR_PARTB)/$(LD_NAME_PARTB).elf: $(OBJS_PARTB)
	$(LD) $(LFLAGS) -o $@ $(OBJS_PARTB) $(LIBLNK)
	$(SZ) $@

# Create build directories if they don’t exist
$(BUILD_DIR_DIMMING):
	mkdir -p $@

# Create build directories if they don’t exist
$(BUILD_DIR_PARTA):
	mkdir -p $@

$(BUILD_DIR_PARTB):
	mkdir -p $@

# Clean all build files
clean:
	rm -rf $(BUILD_DIR_DIMMING) $(BUILD_DIR_PARTA) $(BUILD_DIR_PARTB)
