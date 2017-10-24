#include "dict.h"
#include "api.h"
#include "sigil.h"
#include "log.h"
#include <iostream>

bool hsetCommand(std::deque<std::string> &args)
{
    LOG(Info) << "command (hset)" << std::endl;

    if (args.size() != 3)
    {
        handleArgsError();
        return false;
    }
    std::string dict = args[0];
    std::string key = args[1];
    std::string value = args[2];

    std::shared_ptr<Dict<std::string, std::string>> d =
        Server::getCurrDb()->getDict(dict);
    //find the dict or create a new
    return d->dictSet(key, value);
}

bool hmsetCommand(std::deque<std::string> &args)
{
    LOG(Info) << "command (hmset)" << std::endl;

    if (args.size() % 2 == 0)
    {
        handleArgsError();
        return false;
    }
    std::string dict = args.front();
    args.pop_front();
    std::shared_ptr<Dict<std::string, std::string>> d =
        Server::getCurrDb()->getDict(dict);

    while (args.size())
    {
        std::string key = args.front();
        args.pop_front();

        std::string value = args.front();
        args.pop_front();

        d->dictSet(key, value);
    }

    return true;
}

bool hgetCommand(std::deque<std::string> &args)
{
    LOG(Info) << "command (hget)" << std::endl;

    if (args.size() != 2)
    {
        handleArgsError();
        return false;
    }
    std::string dict = args[0];
    std::string key = args[1];
    std::shared_ptr<Dict<std::string, std::string>> d =
        Server::getCurrDb()->findDict(dict);
    if (d == nullptr)
    {
        std::cout << "(error) no dict \"" << dict << "\"" << std::endl;
        return false;
    }
    std::string res;
    if (d->dictGet(key, res))
    {
        std::cout << key << ":" << res << std::endl;
        return true;
    }
    else
    {
        std::cout << "(error) no key \"" << key << "\"" << std::endl;
        return false;
    }
}

bool hmgetCommand(std::deque<std::string> &args)
{
    LOG(Info) << "command (hmget)" << std::endl;

    if (args.size() <= 1)
    {
        handleArgsError();
        return false;
    }

    std::string dict = args.front();
    args.pop_front();
    std::shared_ptr<Dict<std::string, std::string>> d =
        Server::getCurrDb()->findDict(dict);

    if (d == nullptr)
    {
        std::cout << "(error) no dict \"" << dict << "\"" << std::endl;
        return false;
    }

    while (args.size())
    {
        std::string key = args.front();
        args.pop_front();

        std::string res;
        if (d->dictGet(key, res))
            std::cout << key << ":" << res << std::endl;
    }

    return true;
}

bool hlenCommand(std::deque<std::string> &args)
{
    LOG(Info) << "command (hlen)" << std::endl;

    if (args.size() != 1)
    {
        handleArgsError();
        return false;
    }

    // std::shared_ptr<Dict<std::string, std::string>> d = Server::getCurrDb()->findDict(dict);
    std::string dict = args[0];
    std::shared_ptr<Dict<std::string, std::string>> d =
        Server::getCurrDb()->findDict(dict);

    int length = 0;
    if (d == nullptr)
    {
        std::cout << "(error) no dict \"" << dict << "\"" << std::endl;
        return false;
    }
    else
    {
        length = d->dictLen();
        std::cout << "length:" << length << std::endl;
        return true;
    }
}

bool hclearCommand(std::deque<std::string> &args)
{
    LOG(Info) << "command (hclear)" << std::endl;
    if (args.size() != 1)
    {
        handleArgsError();
        return false;
    }
    std::string dict = args[0];
    std::shared_ptr<Dict<std::string, std::string>> d =
        Server::getCurrDb()->findDict(dict);
    if (d == nullptr)
    {
        std::cout << "(error) no dict \"" << dict << "\"" << std::endl;
        return false;
    }
    else
    {
        d->dictClear();
        return true;
    }
}
