#include "../rbtree.h"

using namespace std;

int main()
{

    {
        int n;
        cin >> n;
        cout << "---------" << n << endl;
        RbTree tree;
        for (int i = 0; i < n; i++)
            tree.insert(i);
        tree.PrintTreeLevel();
    }

    return 0;
}