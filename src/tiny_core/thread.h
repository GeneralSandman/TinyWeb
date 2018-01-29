/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
****************************************
*
*/
 
#ifndef THREAD_T
#define THREAD_T

#include <tiny_base/atomic.h>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <string>
#include <pthread.h>

typedef boost::function<void()> threadFun;
class Thread : boost::noncopyable
{
private:
  std::string m_nName;
  pthread_t m_nThreadId;
  pid_t m_nTid;

  threadFun m_fThreadFun;
  bool m_nStarted;
  bool m_nJoined;

  //m_nFStartThread must be static function
  // why:http://www.xuebuyuan.com/779187.html
  static void *m_fStartThread(void *); //
  void runInThread();
  static AtomicInt32 m_nThreadsNum;

public:
  explicit Thread(const threadFun &, const std::string &name="");
  //constructor don't create thread
  //start() create thread
  void start();
  int join();
  bool started();
  pid_t getTid();
  const std::string &getName();
  static int numCreated();

  ~Thread();
};

#endif // !THREAD_T
