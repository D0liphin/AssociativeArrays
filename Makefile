# ****************************** CHANGE THESE ******************************* #
# all directories that might contain header files
INCLUDE=. ./include
# all directories that might contain source files
SRC=. ./include
# the C++ standard to use
CPP_STD=c++11
# the C standard to use
C_STD=c11
# where should we put the benchmarks? 
BENCHMARK_OUT=./benchmark_results/out.json
# *************************************************************************** #

# some ANSI escape codes
GREEN=\033[0;32m
RED=\033[0;31m
BOLD=\033[1m
RESET=\033[0m

# the compiler to be used for C++
CPPC=g++
# flags for compiling translation units
CPPC_FLAGS=-std=$(CPP_STD) -Wall -Wextra -g $(foreach dir, $(INCLUDE),-I $(dir)) -O3
# the compiler to be used for C
CC=gcc
# flags for compiling translation units for C
CC_FLAGS=-std=$(C_STD) -Wall -Wextra -g $(foreach dir, $(INCLUDE),-I $(dir)) -O3
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
	
$(TARGET)/%.cpp.o: ./%.cpp
	@mkdir -p $(dir ./$@)
	$(CPPC) $(CPPC_FLAGS) -MMD -MP -c -o ./$@ $<

$(TARGET)/%.c.o: ./%.c
	@mkdir -p $(dir ./$@)
	$(CC) $(CC_FLAGS) -MMD -MP -c -o ./$@ $<

build: $(O_FILES) ./target/src/main.c.o Makefile
	@mkdir -p $(TARGET)
	$(CPPC) $(LD_FLAGS) $(O_FILES) ./target/src/main.c.o -o $(TARGET)/main

run:
	@./target/main

print-src: 
	@echo "object files ="$(O_FILES)
	@echo "source files ="$(SRC_FILES)

build-benchmarks: $(O_FILES) Makefile
	@mkdir -p $(TARGET)
	$(CPPC) $(CPPC_FLAGS) ./src/benchmark.cpp $(O_FILES) \
		-isystem ./benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread -o ./target/benchmark

run-benchmarks:
	./target/benchmark --benchmark_out=$(BENCHMARK_OUT)

build-tests: $(O_FILES) ./target/src/test.c.o Makefile
	@mkdir -p $(TARGET)
	$(CPPC) $(LD_FLAGS) $(O_FILES) ./target/src/test.c.o -o $(TARGET)/test

run-tests: 
	@./target/test

# remove target
clean:
	@rm -rf ./target
	@echo "$(GREEN)***$(RESET) All cleaned up, boss! (￣ー￣)ゞ $(GREEN)***\
	$(RESET)"

