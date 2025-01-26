CPP = g++ -std=c++20 -g -Wall -pedantic -Wextra
DEPS_RAYLIB = -L C:/raylib/lib -lraylib -lgdi32 -lwinmm

MAIN_SRC = src/main.cpp

MAIN_EXE = main.exe

all: compile finish open
test_rmse: compile_tests finish

compile:
	$(CPP) $(MAIN_SRC) -I C:/raylib/include -o $(MAIN_EXE) $(DEPS_RAYLIB)

finish:
	@echo -e "\033[0;32m== $(shell date) ==\e[0m"

open:
	@echo -e "\033[0;94mRunning exe...\e[0m"
	@$(MAIN_EXE)
