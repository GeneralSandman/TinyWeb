test:test.o EventLoop.o
	g++ -o test test.o EventLoop.o -pthread

test.o:test.cc
	g++ -std=c++11 -c test.cc -pthread

EventLoop.o:EventLoop.cc
	g++ -std=c++11 -c EventLoop.cc -pthread


clean:
	rm *.o