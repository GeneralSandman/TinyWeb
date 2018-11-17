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

#include <stdio.h>
#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;

std::string file_path = "tinyweb.json";

void generate_user()
{
	boost::property_tree::ptree root;
	boost::property_tree::ptree items;

	boost::property_tree::ptree item1;
	item1.put("ID","1");
	item1.put("Name","wang");
	items.push_back(std::make_pair("1",item1));

	boost::property_tree::ptree item2;
	item2.put("ID","2");
	item2.put("Name","zhang");
	items.push_back(std::make_pair("2",item2));

	boost::property_tree::ptree item3;
	item3.put("ID","3");
	item3.put("Name","li");
	items.push_back(std::make_pair("3",item3));

	root.put_child("user",items);
	boost::property_tree::write_json(file_path,root);
}

using namespace boost::property_tree;

void read_user()
{
	boost::property_tree::ptree root;
	boost::property_tree::ptree items;
	boost::property_tree::read_json<boost::property_tree::ptree>(file_path,root);

	items=root.get_child("develop");
	boost::property_tree::ptree basic = items.get_child("basic");
	boost::property_tree::ptree server = items.get_child("server");
	boost::property_tree::ptree log = items.get_child("log");

    // basic-config
    int processpool = basic.get<int>("processpool", 10);
    int sendfile = basic.get<int>("sendfile", 1);
    std::cout << processpool << std::endl;
    std::cout << sendfile << std::endl;

    // server-config
	for (boost::property_tree::ptree::iterator it=server.begin();it!=server.end();++it)
	{
        int listen = it->second.get<int>("listen", 80);
        std::string servername = it->second.get<std::string>("servername", "");
        std::string www = it->second.get<std::string>("www", "");
        std::string indexpage = it->second.get<std::string>("indexpage", "");
        std::string errorpage = it->second.get<std::string>("errorpage", "");

        std::cout << listen << std::endl;
        std::cout << servername << std::endl;
        std::cout << www << std::endl;
        std::cout << indexpage << std::endl;
        std::cout << errorpage << std::endl;
	}

    // log-config
    std::string level = log.get<std::string>("level", "Info");
    std::string path = log.get<std::string>("path", "");
    std::string debugfile = log.get<std::string>("debugfile", "");
    std::string infofile = log.get<std::string>("infofile", "");
    std::string warnfile = log.get<std::string>("warnfile", "");
    std::string errorfile = log.get<std::string>("errorfile", "");
    std::string fatalfile = log.get<std::string>("fatalfile", "");

    std::cout << level << std::endl;
    std::cout << path << std::endl;
    std::cout << debugfile << std::endl;
    std::cout << infofile << std::endl;
    std::cout << warnfile << std::endl;
    std::cout << errorfile << std::endl;
    std::cout << fatalfile << std::endl;
}

int main()
{
    //generate_user();
    read_user();
    return 0;
}
