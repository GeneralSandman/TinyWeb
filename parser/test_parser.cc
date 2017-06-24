#include "parser.h"

#include <iostream>
#include <unistd.h>
#include <string.h>
using namespace std;

int main()
{

    char buf[4096];
    memset(buf, 0, sizeof(buf));

    int data_recv = 0;
    int data_index = 0;
    int sockfd = 111;
    parser::Parser *par = new parser::Parser();
    while (1)
    {
        data_recv = read(sockfd, buf + data_index, sizeof(buf) - data_index);
        if (data_recv == -1)
        {
        }
        else if (data_recv == 0)
        {
        }
        else
        {
            data_index += data_recv;

            parser::HTTP_CODE result = par->parse_content(buf);
            if (result == parser::HTTP_CODE::NO_REQUEST)
            {
                //contiune recv data
            }
            else if (result == parser::HTTP_CODE::GET_REQUEST)
            {
                //response
            }
            else
            {
                //bad
            }
        }
    }

    delete par;

    return 0;
}