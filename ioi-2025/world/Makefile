submit:
	/opt/homebrew/bin/g++-15 -o checker.o checker.cpp
	/opt/homebrew/bin/g++-15 -o grader.o grader.cpp worldmap.cpp
	./grader.o < input.txt > output.txt
	./checker.o input.txt output.txt
