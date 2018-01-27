link = g++ -o
gcc11 = g++ -std=c++11 -c
gcc11o = $(gcc11) -o


lib = api channel condition currentthread epoller eventloop exception \
	log time timer timerqueue socket netaddress connection server accepter \
	process processpool reader configer buffer protocol factory parser responser http \
	sharedmemory semaphore master
lib.o = $(foreach file,$(lib),$(file).o)
lib.cc = $(foreach file,$(lib),$(file).cc)

TinyWeb:main.cc $(lib.cc)
	$(gcc11) main.cc
	$(gcc11) $(lib.cc)
	$(link) TinyWeb main.o $(lib.o) -lpthread
	rm *.o
	
clean:
	rm *.o