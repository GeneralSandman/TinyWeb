#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <string>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

typedef boost::function<void()> Fun;
class Process : boost::noncopyable
{
private:
  std::string m_nName; //parent process is "main"
  pid_t m_nPid;

  bool m_nStarted;
  bool m_nExited;

public:
  explicit Process(const std::string &name = "");
  //constructor don't create thread
  //start() create thread
  void start();
  int join();
  bool started();
  pid_t getPid();
  const std::string &getName();

  ~Process();
};

#endif // !PROCESS_H
