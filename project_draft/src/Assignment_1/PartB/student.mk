##### Student Makefile for the ESS deliverables #####
# Change this file to match your requirements

# Name of your project
LD_NAME = Ass1PartB

# Add lists of space separated source files
# Own sources, e.g. main.c
SRCS = PartB.c
# Library sources, e.g. xmc_gpio.c
LIBSRCS = 
# Precompiled libraries, e.g. -lm for math functions
LIBLNK =

# Change this line if the path to your XMC-Library differs, it will be overwritten before grading to
# match the system the grading is performed on.
XMC_LIBDIR = /opt/XMClib/XMC_Peripheral_Library_v2.1.16

# Language dialect
SCFLAGS  = -std=gnu99
# Optimization level, remember that enabling optimization will stirr up the assembly code and thus
# debugging is more difficult
SCFLAGS += -O0
# If you like, you can enable even more warnings, e.g. -Wextra, but for grading -Wall will be used
SCFLAGS += -Wall


# Define variable to tell compiler that my own include path is available
SCFLAGS += -DINCLUDE_PATH_AVAILABLE
# Include paths for your own headers, e.g. -I./inc
SCFLAGS += $(foreach dir, $(shell find ../../../include -type d), -I$(dir))