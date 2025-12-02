F ?= test

create:
	touch $(F).cpp
	touch $(F).in
	touch $(F).out

run:
	g++ -O3 -std=c++17 $(F).cpp -o $(F).o && cat $(F).in | ./$(F).o > $(F).out

hash:
	/opt/homebrew/bin/g++-15 -E -dD -P -x c++ $(F).cpp \
	| tr -d '[:space:]' \
	| md5 -q \
	| cut -c1-6
