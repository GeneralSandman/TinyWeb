/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#ifndef TINYWEB_STATUS_H
#define TINYWEB_STATUS_H

extern int status_quit_softly; //QUIT
extern int status_terminate;   //TERM,INT
extern int status_exiting;
extern int status_restart;
extern int status_reconfigure; //HUP,reboot
extern int status_child_quit;  //CHLD

#endif //!TINYWEB_STATUS_H
