ll = g++ -o
gg = g++ -std=c++11 -c -o


link = g++ -o
gcc11 = g++ -std=c++11 -c
gcc11o = $(gcc11) -o


lib = api channel condition currentthread epoller eventloop exception \
	log time timer timerqueue socket netaddress
lib.o = $(foreach file,$(lib),$(file).o)
lib.cc = $(foreach file,$(lib),../$(file).cc)

lib:$(lib.cc)
	$(gcc11) $(lib.cc)
base_test:base_test.o $(lib.o)
	$(link) base_test base_test.o $(lib.o)
api_test:api_test.o api.o
	$(ll) api_test api_test.o api.o
thread_test:thread_test.o thread.o currentthread.o api.o
	$(ll) thread_test thread_test.o thread.o threadpool_test.o currentthread.o api.o -lpthread
threadpool_test:threadpool_test.o thread.o threadpool.o currentthread.o api.o
	$(ll) threadpool_test threadpool_test.o thread.o threadpool.o currentthread.o api.o -lpthread
time_test:time_test.o time.o
	$(link) time_test time_test.o time.o
socket_test:socket_test.o $(lib.o)
	$(link) socket_test socket_test.o $(lib.o)

base_test.o:base_test.cc
	$(gcc11o) base_test.o base_test.cc
api_test.o:api_test.cc
	$(gg) api_test.o api_test.cc
thread_test.o:thread_test.cc
	$(gg) thread_test.o thread_test.cc
threadpool_test.o:threadpool_test.cc
	$(gg) threadpool_test.o threadpool_test.cc
time_test.o:time_test.cc
	$(gcc11) time_test.cc
socket_test.o:socket_test.cc
	$(gcc11) socket_test.cc



clean:
	rm *.o

cleanall:
	rm *_test
	rm *.o