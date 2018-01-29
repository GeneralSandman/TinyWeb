/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---Configer Class---
*We need to upgrade this class
*in order to handle config file error
****************************************
*
*/

#ifndef CALL_BACK_H
#define CALL_BACK_H

#include <tiny_core/time.h>

#include <boost/function.hpp>

class Buffer;
class Connection;

typedef boost::function<void()> TimerCallback;
typedef boost::function<void(Connection *)> ConnectionCallback;
typedef boost::function<void(Connection *,
                             Buffer *buf,
                             Time)>
    MessageCallback;
typedef boost::function<void(Connection *)> WriteCompleteCallback;
typedef boost::function<void(Connection *)> CloseCallback;

#endif // !CALL_BACK_H
