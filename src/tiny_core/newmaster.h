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

#ifndef NEW_MASTER_H
#define NEW_MASTER_H

#include <map>

class Worker;
class ProcessPool;

class NewMaster
{
private:
  std::map<int, Worker *> m_nWorkers;
  ProcessPool *m_pProcessPool;

public:
  NewMaster();
  void startWork();
  void restartWork();

  void stopAllWorkers();
  void killAllWorkers();

  Worker *createNewWorker();
  ~NewMaster();
};

#endif // !NEW_MASTER_H
