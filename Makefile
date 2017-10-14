link = g++ -o
gcc11 = g++ -std=c++11 -c
gcc11o = $(gcc11) -o


lib = api channel condition currentthread epoller eventloop exception \
	log time timer timerqueue socket netaddress connection server accepter \
	process processpool configer reader master protocol
lib.o = $(foreach file,$(lib),$(file).o)
lib.cc = $(foreach file,$(lib),$(file).cc)

TinyWeb:main.cc $(lib.cc)
	$(gcc11) main.cc
	$(gcc11) $(lib.cc)
	$(link) TinyWeb main.o $(lib.o)
	rm *.o
	
clean:
	rm *.o