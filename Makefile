BUILD_DIR := ./build/
SRC_DIR := ./src/

main.exe: main.o btree.o
	gcc -g -o $(BUILD_DIR)main.exe $(BUILD_DIR)btree.o $(BUILD_DIR)main.o

main.o : $(SRC_DIR)main.c $(SRC_DIR)btree.h
	gcc -g -c $(SRC_DIR)main.c -o $(BUILD_DIR)main.o

btree.o : $(SRC_DIR)btree.c $(SRC_DIR)btree.h
	gcc -g -c $(SRC_DIR)btree.c -o $(BUILD_DIR)btree.o

clean:
	rm ./build/*

