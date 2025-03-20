BUILD_DIR := ./build/
SRC_DIR := ./src/
TESTS_DIR := ./tests/
EXE_EXT := .exe

main$(EXE_EXT) : main.o btree.o
	gcc -g -o $(BUILD_DIR)main$(EXE_EXT) $(BUILD_DIR)btree.o $(BUILD_DIR)main.o

main.o : $(SRC_DIR)main.c $(SRC_DIR)btree.h
	gcc -g -c $(SRC_DIR)main.c -o $(BUILD_DIR)main.o

btree.o : $(SRC_DIR)btree.c $(SRC_DIR)btree.h
	gcc -g -c $(SRC_DIR)btree.c -o $(BUILD_DIR)btree.o

tests.o : $(TESTS_DIR)rktest.h $(TESTS_DIR)tests.c $(SRC_DIR)btree.h
	gcc -g -c $(TESTS_DIR)tests.c -o $(BUILD_DIR)tests.o

tests$(EXE_EXT) : tests.o btree.o
	gcc -g -o $(BUILD_DIR)tests$(EXE_EXT) $(BUILD_DIR)btree.o $(BUILD_DIR)tests.o

tests : tests$(EXE_EXT)
	$(BUILD_DIR)tests$(EXE_EXT)

run : main$(EXE_EXT)
	$(BUILD_DIR)main$(EXE_EXT)

clean:
	rm ./build/*

