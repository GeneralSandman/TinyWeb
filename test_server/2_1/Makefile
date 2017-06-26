test_poll:test.o processpoll.o api.o
	g++ -o test_poll test.o processpoll.o api.o
test.o:./test.cc
	g++ -std=c++11 -c test.cc

processpoll.o:./processpoll.cc
	g++ -std=c++11 -c processpoll.cc

api.o:../../api/api.cc
	g++ -std=c++11 -c ../../api/api.cc

clean:
	rm *.o

