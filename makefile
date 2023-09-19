compile:
	g++ -shared -std=c++17 core/*.cpp -o core.so -ldl -fPIC
	g++ test.cpp -o test

run:
	LD_PRELOAD=`realpath core.so` ./test
