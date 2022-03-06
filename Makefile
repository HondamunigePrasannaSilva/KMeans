
all:  clean main

main:
	g++  main.cpp header.cpp -fopenmp -o main
	g++  seq.cpp header.cpp -o seq

clean: 
	-rm main.exe seq.exe