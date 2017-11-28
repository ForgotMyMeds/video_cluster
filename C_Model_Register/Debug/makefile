all:
	g++ -Os -g3 -Wall -o cmodel.so -shared -fPIC -lboost_python -I/usr/include/python2.7/ `pkg-config --libs --cflags opencv` -fpermissive ../src/*.cpp 
