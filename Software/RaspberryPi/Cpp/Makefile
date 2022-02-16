#########################################################################
# File name: Makefile (Cpp/)
# Dev: GitHub@Rr42
# Code version: v1.0
# Description: 
# This code ficitilates easy compilation of the Raspberry Pi
#  8x11 LED display C++ examples.
#########################################################################

# Ref: https://stackoverflow.com/questions/17834582/run-make-in-each-subdirectory
# Decide whether the commands will be shwon or not
VERBOSE = FALSE

# Subdirectories/libs to make
LIBS = LED8x11_display_lib

# Subdirectories/apps to make
APPS = LED8x11_display_example1 LED8x11_display_example2

TARGETS = $(LIBS) $(APPS)

# Hide or not the calls depending of VERBOSE
ifeq ($(VERBOSE),TRUE)
	HIDE =  
else
	HIDE = @
endif

TOPTARGETS := all clean

$(TOPTARGETS): $(TARGETS)
$(TARGETS):
	$(eval export VERBOSE := $(VERBOSE))
	$(HIDE)echo '####################################'
	$(HIDE)echo Processing $@
	$(HIDE)echo '####################################'
	$(HIDE)$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(TARGETS)
