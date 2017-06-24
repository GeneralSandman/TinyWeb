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
    parser::Parser *par = new parser::Parser(1);
    while (1)
    {
        data_recv = par->recv_data_continue();
        if (data_recv == -1)
        {
            break;
        }
        else if (data_recv == 0)
        {
            break;
        }
        else
        {
            parser::HTTP_CODE result = par->parse_content();
            if (result == parser::HTTP_CODE::NO_REQUEST)
            {
                //contiune recv data
                continue;
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