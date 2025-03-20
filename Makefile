BUILD_DIR := ./build/

main.exe: main.o btree.o
	gcc -g -o $(BUILD_DIR)main.exe $(BUILD_DIR)btree.o $(BUILD_DIR)main.o

main.o : main.c btree.h
	gcc -g -c main.c -o $(BUILD_DIR)main.o

btree.o : btree.c btree.h
	gcc -g -c btree.c -o $(BUILD_DIR)btree.o

clean:
	rm ./build/*

