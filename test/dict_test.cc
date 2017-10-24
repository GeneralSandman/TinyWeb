#include "../dict.h"
#include "../log.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    {
        string d = "db";
        Dict<string, string> newdict(d);

        for (int i = 0; i < 10; i++)
        {
            string key, value;
            key += char(98 + i);
            value = key;
            cout << key << ":" << value << endl;

            newdict.dictSet(key, value);
        }
        cout << "len:" << newdict.dictLen() << endl;

        DictIter<string, string> iter(&newdict);
        DictEntry<string, string> *next = nullptr;
        while ((next = iter.getDictNext()) != nullptr)
        {
            cout << next->getKey() << ":" << next->getValue() << endl;
        }
        cout<<"----\n";
        iter.reset2Begin();
        while ((next = iter.getDictNext()) != nullptr)
        {
            cout << next->getKey() << ":" << next->getValue() << endl;
        }
    }

    return 0;
}