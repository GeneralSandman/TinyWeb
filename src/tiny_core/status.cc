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

#include <tiny_core/status.h>

int status_quit_softly = 0; //QUIT
int status_terminate = 0;   //TERM,INT
int status_exiting = 0;
int status_restart = 0;
int status_reconfigure = 0; //HUP,reboot
int status_child_quit = 0;  //CHLD
