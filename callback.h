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

#include "time.h"

#include <boost/function.hpp>

class Buffer;
class TcpConnection;

typedef boost::function<void()> TimerCallback;
typedef boost::function<void(const TcpConnection *)> ConnectionCallback;
typedef boost::function<void(const TcpConnection *,
                             Buffer *buf,
                             Time)>
    MessageCallback;
typedef boost::function<void(const TcpConnection *)> CloseCallback;

#endif // !CALL_BACK_H
