#########################################################################
# File name: Makefile (LED8x11_display_example1/)
# Dev: GitHub@Rr42
# Code version: v1.0
# Description: 
# This code ficitilates easy compilation of the Raspberry Pi
#  8x11 LED display C++ example1.
#########################################################################

# Ref: https://riptutorial.com/makefile/example/21376/building-from-different-source-folders-to-different-target-folders
# Set project directory one level above of Makefile directory. $(CURDIR) is a GNU make variable containing the path to the current working directory
PROJDIR := $(realpath $(CURDIR)/..)
SOURCEDIR := $(PROJDIR)
BUILDDIR := $(PROJDIR)/build

# Name of the final executable
TARGET = example1

# Decide whether the commands will be shwon or not
# VERBOSE = TRUE

# Create the list of directories
DIRS = LED8x11_display_example1
SOURCEDIRS = $(foreach dir, $(DIRS), $(addprefix $(SOURCEDIR)/, $(dir)))
TARGETDIRS = $(foreach dir, $(DIRS), $(addprefix $(BUILDDIR)/, $(dir)))

# Common Library headers
INCLUDEDIR = $(PROJDIR)/LED8x11_display_lib

# Generate the GCC includes parameters by adding -I before each source folder
INCLUDES = -I$(INCLUDEDIR) $(foreach dir, $(SOURCEDIRS), $(addprefix -I, $(dir)))

# Libraries to link
LIBS = -lled8x11display -lwiringPi
LIBDIR = -L$(BUILDDIR)/lib -L/usr/lib

# Add this list to VPATH, the place make will look for the source files
VPATH = $(SOURCEDIRS)

# Create a list of *.cpp sources in DIRS
SOURCES = $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.cpp))

# Define objects for all sources
OBJS := $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCES:.cpp=.o))

# Define dependencies files for all objects
DEPS = $(OBJS:.o=.d)

# Name the compiler
CXX = g++

# OS specific part
ifeq ($(OS),Windows_NT)
	RM = del /F /Q 
	RMDIR = -RMDIR /S /Q
	MKDIR = -mkdir
	ERRIGNORE = 2>NUL || true
	SEP=\\
else
	RM = rm -rf 
	RMDIR = rm -rf 
	MKDIR = mkdir -p
	ERRIGNORE = 2>/dev/null
	SEP=/
endif

# Remove space after separator
PSEP = $(strip $(SEP))

# Hide or not the calls depending of VERBOSE
ifeq ($(VERBOSE),TRUE)
	HIDE =  
else
	HIDE = @
endif

# Define the function that will generate each rule
define generateRules
$(1)/%.o: %.cpp
	$(HIDE)@echo Building $$@
	$(HIDE)$(CXX) -c $$(INCLUDES) -o $$(subst /,$$(PSEP),$$@) $$(subst /,$$(PSEP),$$<) -MMD
endef

.PHONY: all clean directories 

all: directories $(TARGET)

$(TARGET): $(OBJS)
	$(HIDE)echo Linking $@
	$(HIDE)$(CXX) $(OBJS) -o $(BUILDDIR)/$(TARGET) $(LIBDIR) $(LIBS)

# Include dependencies
-include $(DEPS)

# Generate rules
$(foreach targetdir, $(TARGETDIRS), $(eval $(call generateRules, $(targetdir))))

directories: 
	$(HIDE)$(MKDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)

# Remove all objects, dependencies and executable files generated during the build
clean:
	$(HIDE)$(RMDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)
	$(HIDE)$(RM) $(BUILDDIR)/$(TARGET) $(ERRIGNORE)
	$(HIDE)echo '##################'
	$(HIDE)@echo Cleaning done !
	$(HIDE)echo '##################'
