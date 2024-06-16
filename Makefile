# ****************************** CHANGE THESE ******************************* #
# all directories that might contain header files
INCLUDE=. ./include
# all directories that might contain source files
SRC=. ./src ./include
# the C++ standard to use
CPP_STD=c++11
# the C standard to use
C_STD=c11
# *************************************************************************** #

# some ANSI escape codes
GREEN=\033[0;32m
RED=\033[0;31m
BOLD=\033[1m
RESET=\033[0m

# the compiler to be used for C++
CPPC=g++
# flags for compiling translation units
CPPC_FLAGS=-std=$(CPP_STD) -Wall -Wextra -g $(foreach dir, $(INCLUDE),-I $(dir))
# the compiler to be used for C
CC=gcc
# flags for linking
LD_FLAGS=
# where all generated files are stored
TARGET=./target
# name of the built executable
EXEC=main
# name of all files that are any kind of source file
SRC_EXTS=c cpp

# 1 arg: convert this source file name into an object file name
define make_o_files
$(foreach ext,$(SRC_EXTS),\
	$(patsubst ./%.$(ext),$(TARGET)/%.$(ext).o,\
		$(filter %.$(ext),$(1))))
endef

CPP_FILES=$(foreach dir,$(SRC),\
				$(wildcard $(dir)/*.cpp))

C_FILES=$(foreach dir,$(SRC),\
				$(wildcard $(dir)/*.c))

SRC_FILES=$(C_FILES)$(CPP_FILES)

O_FILES=$(call make_o_files,$(SRC_FILES))

# list of all .d files
D_FILES=$(patsubst %.o,%.d,$(O_FILES)) 
-include $(D_FILES)

all: build

# build the project, creating ./target/main
build: $(TARGET)/main

# continuation of the above, but making sure we don't relink each time
$(TARGET)/main: $(O_FILES) Makefile
	@mkdir -p $(TARGET)
# only compile if there is actually something to compile
ifdef SRC_FILES

	$(CPPC) $(LD_FLAGS) -o $(TARGET)/$(EXEC) $(O_FILES)
endif
	@echo "$(GREEN)***$(RESET) done! \(^-^)/ $(GREEN)***$(RESET)"
	
$(TARGET)/%.cpp.o: ./%.cpp
	@mkdir -p $(dir ./$@)
	$(CPPC) $(CPPC_FLAGS) -MMD -MP -c -o ./$@ $<

$(TARGET)/%.c.o: ./%.c
	@mkdir -p $(dir ./$@)
	$(CC) $(CC_FLAGS) -MMD -MP -c -o ./$@ $<

# runs the main program
run:
	@./target/main

print-src: 
	@echo "object files ="$(O_FILES)
	@echo "source files ="$(SRC_FILES)

# remove target
clean:
	@rm -rf ./target
	@echo "$(GREEN)***$(RESET) All cleaned up, boss! (￣ー￣)ゞ $(GREEN)***\
	$(RESET)"

# # prints help about the usage of this Makefile
# help:
# 	@echo ""
# 	@echo "Simple C++ build utility"
# 	@echo ""
# 	@echo "USAGE:"
# 	@echo "    make [COMMAND]"
# 	@echo ""
# 	@echo "COMMAND:"
# 	@echo "    init         Create the recommended project structure"
# 	@echo "    build        Compile all source files into ./target/main"
# 	@echo "    clean        Remove the target directory"
# 	@echo "    run          Shorthand for ./target/main"
# 	@echo "    help         Display this message"
# 	@echo "    print-src    Print files being used by olibuild"
# 	@echo "    build-tests  Compiles all tests (for now, this will always relink)"
# 	@echo "    run-tests    Runs all built tests"
# 	@echo ""
# 	@echo "NOTE: if all your tests are in ./tests you can clean just the binaries"
# 	@echo "      generated from your tests with `sudo rm -rf ./target/tests`"
# 	@echo ""

