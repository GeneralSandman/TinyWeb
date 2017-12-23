#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> res;

    for (int i = 0; i < 20; i++)
    {
        cout << "insert\n";
        res.push_back(i);
    }
    return 0;
}