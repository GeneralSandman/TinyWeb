1_0test:1_0test.o api.o parser.o
	g++ -o 1_0test 1_0test.o api.o parser.o
1_0test.o:1_0test.cc
	g++ -std=c++11 -c 1_0test.cc

api.o: ../../api/api.cc
	g++ -std=c++11 -c ../../api/api.cc

parser.o:../../parser/parser.cc
	g++ -std=c++11 -c ../../parser/parser.cc

clean:
	rm 1_0test 1_0test.o api.o parser.o
