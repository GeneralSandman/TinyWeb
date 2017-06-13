test:./api/api.cc ./api/server.cc ./server/server.cc
	g++ -std=c++11 -o test ./api/api.cc ./api/server.cc ./server/server.cc