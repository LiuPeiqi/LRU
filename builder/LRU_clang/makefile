INCLUDE_PATH = -I../../src
BUILD_DIR = ./out

CXXFLAG = --std=c++17

CC = clang++ $(INCLUDE_PATH) $(CXXFLAG)

test_lru:
	mkdir -p $(BUILD_DIR)

	$(CC) -o $(BUILD_DIR)/$@  ../../src/test_lru.cpp
