#include "../list.h"
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

int main()
{

    {
        List<string> l("test");
        for (int i = 0; i < 5; i++)
            l.add_head("a");

        std::cout << "---------\n";
        std::cout << "test getByIndex()\n";
        string value;
        for (int i = 0; i < 10; i++)
        {
            if (l.getByIndex(i, value))
                std::cout << "get index " << i << ":" << value << std::endl;
        }

        std::cout << "---------\n";
        std::cout << "test setByIndex()\n";
        for (int i = 0; i < 10; i++)
        {
            if (l.setByIndex(i, "value"))
                std::cout << "set index " << i << ":"
                          << "value" << std::endl;
        }

        std::cout << "---------\n";
        std::cout << "test getByRange()\n";
        vector<string> res;
        if (l.getByRange(res, 0, 4))
        {
            for (auto t : res)
                cout << t << " ";
            cout << endl;
        }

        std::cout << "---------\n";
        std::cout << "test getAll()\n";
        vector<string> aa;
        if (l.getAll(aa))
        {
            for (auto t : aa)
                cout << t << " ";
            cout << endl;
        }
    }

    return 0;
}