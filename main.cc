/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

#include "master.h"

#include <iostream>
#include <getopt.h>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
    {
        string config_file = "/TinyWeb.conf";
        //default config file

        //parse arg line
        struct option longopts[] = {
            {"configfile", 1, NULL, 'c'},
            {0, 0, 0, 0},
        };
        const char *short_options = "c:";
        int c;
        while ((c = getopt_long(argc, argv, short_options, longopts, NULL)) != -1)
        {
            if (c == 'c')
            {
                config_file = optarg;
            }
        }
        //

        //check config file
        if (access(config_file.c_str(), 0) == -1)
        {
            std::cout << "The file(" << config_file << ") doesn't exist" << std::endl;
            exit(-1);
        }

        Master master(config_file);
        master.start();
    }

    return 0;
}