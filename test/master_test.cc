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

#include "../master.h"

#include <iostream>
#include <getopt.h>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
    {
        //default config file
        string config_file = "/home/li/TinyWeb/TinyWeb.conf";
        bool debug = false;

        //parse arg line
        struct option longopts[] = {
            {"configfile", 1, NULL, 'c'},
            {"debug", 0, NULL, 'd'},
            {0, 0, 0, 0},
        };
        const char *short_options = "c:d";
        int c;
        while ((c = getopt_long(argc, argv, short_options, longopts, NULL)) != -1)
        {
            if (c == 'c')
            {
                config_file = optarg;
            }
            else if (c == 'd')
            {
                debug = true;
            }
        }
        //

        //check config file
        if (access(config_file.c_str(), 0) == -1)
        {
            std::cout << "The file(" << config_file << ") doesn't exist" << std::endl;
            exit(-1);
        }

        Master master(config_file,debug);
        master.start();
    }

    return 0;
}