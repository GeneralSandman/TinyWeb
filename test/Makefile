link = g++ -o
gcc11 = g++ -std=c++11 -c
gcc11o = $(gcc11) -o

lib = api channel condition currentthread epoller eventloop exception \
	log time timer timerqueue socket netaddress connection server accepter \
	process processpool reader configer buffer protocol factory parser responser http \
	sharedmemory semaphore master worker defer memorypool
lib.o = $(foreach file,$(lib),$(file).o)
lib.cc = $(foreach file,$(lib),../$(file).cc)

lib:$(lib.cc)
	$(gcc11) $(lib.cc)
base_test:base_test.o $(lib.o)
	$(link) base_test base_test.o $(lib.o)
api_test:api_test.o api.o
	$(link) api_test api_test.o api.o
thread_test:thread_test.o thread.o currentthread.o api.o
	$(link) thread_test thread_test.o thread.o threadpool_test.o currentthread.o api.o -lpthread
threadpool_test:threadpool_test.o thread.o threadpool.o currentthread.o api.o
	$(link) threadpool_test threadpool_test.o thread.o threadpool.o currentthread.o api.o -lpthread
process_test:process_test.o $(lib.o)
	$(link) process_test process_test.o $(lib.o) -lpthread
processpool_test:processpool_test.o $(lib.o)
	$(link) processpool_test processpool_test.o $(lib.o) -lpthread
memorypool_test:memorypool_test.o $(lib.o)
	$(link) memorypool_test memorypool_test.o $(lib.o) -lpthread
time_test:time_test.o time.o
	$(link) time_test time_test.o time.o
timerqueue_test:timerqueue_test.o $(lib.o)
	$(link) timerqueue_test timerqueue_test.o $(lib.o) -lpthread
socket_test:socket_test.o $(lib.o)
	$(link) socket_test socket_test.o $(lib.o)
accepter_test:accepter_test.o $(lib.o)
	$(link) accepter_test accepter_test.o $(lib.o)
server_test:server_test.o $(lib.o)
	$(link) server_test server_test.o $(lib.o) -lpthread
master_test:master_test.o $(lib.o)
	$(link) master_test master_test.o $(lib.o) -lpthread
reader_test:reader_test.o $(lib.o)
	$(link) reader_test reader_test.o $(lib.o) -lpthread
configer_test:configer_test.o $(lib.o)
	$(link) configer_test configer_test.o $(lib.o) -lpthread
daemon_test:daemon_test.o $(lib.o)
	$(link) daemon_test daemon_test.o $(lib.o) -lpthread
sharedmemory_consume_test:sharedmemory_consume_test.o $(lib.o)
	$(link) sharedmemory_consume_test sharedmemory_consume_test.o $(lib.o) -lpthread
sharedmemory_product_test:sharedmemory_product_test.o $(lib.o)
	$(link) sharedmemory_product_test sharedmemory_product_test.o $(lib.o) -lpthread
buffer_test:buffer_test.o $(lib.o)
	$(link) buffer_test buffer_test.o $(lib.o) -lpthread
parser_test:parser_test.o $(lib.o)
	$(link) parser_test parser_test.o $(lib.o) -lpthread
log_test:log_test.o
	$(link) log_test log_test.o log.o
protocol_test:protocol_test.o
	$(link) protocol_test protocol_test.o $(lib.o) -lpthread
defer_test:defer_test.o
	$(link) defer_test defer_test.o $(lib.o) -lpthread
test:test.o $(lib.o)
	$(link) test test.o $(lib.o) -lpthread



base_test.o:base_test.cc
	$(gcc11) base_test.cc
api_test.o:api_test.cc
	$(gcc11) api_test.cc
thread_test.o:thread_test.cc
	$(gcc11) thread_test.cc
threadpool_test.o:threadpool_test.cc
	$(gcc11) threadpool_test.cc
process_test.o:process_test.cc
	$(gcc11) process_test.cc
processpool_test.o:processpool_test.cc
	$(gcc11) processpool_test.cc
memorypool_test.o:memorypool_test.cc
	$(gcc11) memorypool_test.cc
time_test.o:time_test.cc
	$(gcc11) time_test.cc
timerqueue_test.o:timerqueue_test.cc
	$(gcc11) timerqueue_test.cc
socket_test.o:socket_test.cc
	$(gcc11) socket_test.cc
accepter_test.o:accepter_test.cc
	$(gcc11) accepter_test.cc
server_test.o:server_test.cc
	$(gcc11) server_test.cc
master_test.o:master_test.cc
	$(gcc11) master_test.cc	
configer_test.o:configer_test.cc
	$(gcc11) configer_test.cc
daemon_test.o:daemon_test.cc
	$(gcc11) daemon_test.cc
reader_test.o:reader_test.cc
	$(gcc11) reader_test.cc
sharedmemory_consume_test.o:sharedmemory_consume_test.cc
	$(gcc11) sharedmemory_consume_test.cc
sharedmemory_product_test.o:sharedmemory_product_test.cc
	$(gcc11) sharedmemory_product_test.cc
buffer_test.o:buffer_test.cc
	$(gcc11) buffer_test.cc
parser_test.o:parser_test.cc
	$(gcc11) parser_test.cc
log_test.o:log_test.cc
	$(gcc11) log_test.cc
protocol_test.o:protocol_test.cc
	$(gcc11) protocol_test.cc
defer_test.o:defer_test.cc
	$(gcc11) defer_test.cc
test.o:test.cc
	$(gcc11) test.cc




clean:
	rm *.o

cleanall:
	rm *_test
	rm *.o