test:./api/api.cc ./server/basic_server.cc ./test_server.cc
	g++ -std=c++11 -o test ./api/api.cc ./server/basic_server.cc ./test_server.cc
