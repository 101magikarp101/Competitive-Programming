run:
	g++ -O3 -std=c++17 -Wl,-stack_size -Wl,0x20000000 -o solution.o grader.cpp triples.cpp
	./solution.o < input.txt > output.txt