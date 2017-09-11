ll = g++ -o
gg = g++ -std=c++11 -c -o

api_test:api_test.o api.o
	$(ll) api_test api_test.o api.o
thread_test:thread_test.o thread.o currentthread.o api.o -lpthread
	$(ll) thread_test thread_test.o thread.o currentthread.o api.o -lpthread
threadpool_test:threadpool_test.o
	$(ll) threadpool_test threadpool_test.o

api_test.o:api_test.cc
	$(gg) api_test.o api_test.cc
thread_test.o:thread_test.cc
	$(gg) thread_test.o thread_test.cc
threadpool_test.o:threadpool_test.cc
	$(gg) threadpool.o threadpool_test.cc

api.o:../api.cc
	$(gg) api.o ../api.cc
atomic.o:../atomic.cc
	$(gg) atomic.o ../atomic.cc
condition.o:../condition.cc
	$(gg) condition.o ../condition.cc
currentthread.o:../currentthread.cc
	$(gg) currentthread.o ../currentthread.cc
exception.o:../exception.cc
	$(gg) exception.o ../exception.cc
log.o:../log.cc
	$(gg) log.o ../log.cc
thread.o:../thread.cc
	$(gg) thread.o ../thread.cc
threadpool.o:../threadpool.cc
	$(gg) threadpool.o:../threadpool.cc


clean:
	rm *.o