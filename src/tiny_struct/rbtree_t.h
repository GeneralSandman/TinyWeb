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

#ifndef Rb_TREE_T
#define Rb_TREE_T

#include <tiny_base/log.h>

#include <iostream>
#include <vector>

typedef enum { red,
    black } color;

class RbTree;

class RbTreeNode
{
    private:
        color m_nColor;
        int m_nValue;
        RbTreeNode *m_pParent;
        RbTreeNode *m_pLeft;
        RbTreeNode *m_pRight;

    public:
        RbTreeNode(const int &value, color co = red)
        {
            m_nColor = co;
            m_nValue = value;
            m_pParent = m_pLeft = m_pRight = nullptr;
            LOG(Debug) << "class RbTreeNode constructor" << std::endl;
        }
        ~RbTreeNode()
        {
            LOG(Debug) << "class RbTreeNode destructor" << std::endl;
        }
        friend class RbTree;
};

class RbTree
{
    private:
        RbTreeNode *m_pRoot;
        int m_nNumber;

        RbTreeNode *m_fFindNode(const int &);

        void m_fInsertNode(RbTreeNode *);
        void m_fFixAfterInsert(RbTreeNode *);

        void m_fDeleteNode(RbTreeNode *);
        void m_fFixAfterDelete(RbTreeNode *, RbTreeNode *);

        // RbTreeNode *m_fFindLeftSubTreeMaxNode(RbTreeNode *);
        // RbTreeNode *m_fFindRightSubTreeMinNode(RbTreeNode *);
        void m_fDeleteAllNode(RbTreeNode *node, int &);
        void m_fLeftRotate(RbTreeNode *node);
        void m_fRightRotate(RbTreeNode *node);

    public:
        RbTree();
        ~RbTree();
        bool find(const int &);
        void insert(const int &);
        // void deleteNode(const int &);
        int getNumber(void) { return m_nNumber; }

        typedef std::pair<int, color> entry;

        void m_fLevel(std::vector<std::vector<entry>> &result, RbTreeNode *node, int level)
        {
            if (node == nullptr)
                return;
            if ((unsigned int)level == result.size())
                result.push_back(std::vector<entry>());
            result[level].push_back(entry(node->m_nValue, node->m_nColor));
            m_fLevel(result, node->m_pLeft, level + 1);
            m_fLevel(result, node->m_pRight, level + 1);
        }
        void PrintTreeLevel()
        {
            std::vector<std::vector<entry>> result;
            m_fLevel(result, m_pRoot, 0);
            if (result.empty())
                std::cout << "empty\n";
            for (unsigned int i = 0; i < result.size(); i++)
            {
                std::cout << "level " << i + 1 << ":";
                for (auto t : result[i])
                {
                    char c;
                    if (t.second == red)
                        c = 'R';
                    if (t.second == black)
                        c = 'B';

                    std::cout << t.first << c << " ";
                }
                std::cout << std::endl;
            }
        }
};

#endif //!RBTREE_T_H
