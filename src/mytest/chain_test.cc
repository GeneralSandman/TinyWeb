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

#include <tiny_http/http_chain.h>
#include <tiny_http/http_buffer.h>

#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::string str = "helloworld+";
    std::vector<std::string> strs;
    for (auto i=0; i<10 ;i++)
        strs.push_back(str);

    http_chain_t * chain_head = nullptr;

    http_chain_t *curr = chain_head;
    http_chain_t *pre = nullptr;

    for (auto i=0; i<10; i++)
    {
        if (nullptr == curr)
        {
            curr = new http_chain_t;
            if(nullptr == chain_head)
                chain_head = curr;
            curr->buffer.begin = strs[i].c_str();
            curr->buffer.end = strs[i].c_str() + strs[i].size();
            curr->next = nullptr;
        }

        if (nullptr != pre)
        {
            pre->next = curr;
        }

        pre = curr;
        curr = curr->next;
    }

    curr = chain_head;

    while(nullptr != curr)
    {
        printf("%.*s", curr->buffer.end - curr->buffer.begin, curr->buffer.begin);
        curr = curr->next;
    }

    curr = chain_head;
    while(nullptr != curr)
    {
        pre = curr->next;
        delete curr;
        curr = pre;
    }

    return 0;
}
