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

#include <tiny_base/log.h>
#include <tiny_struct/rbtree_t.h>

#include <iostream>

RbTreeNode *RbTree::m_fFindNode(const int &value)
{
    if (m_pRoot == nullptr)
    {
        return nullptr;
    }

    RbTreeNode *node = m_pRoot;
    while (node)
    {
        if (value < node->m_nValue)
            node = node->m_pLeft;
        else if (value > node->m_nValue)
            node = node->m_pRight;
        else
            break;
    }
    return node;
}

void RbTree::m_fInsertNode(RbTreeNode *newnode)
{
    RbTreeNode *pre = m_pRoot, *insert = m_pRoot;
    int newvalue = newnode->m_nValue;
    while (insert != nullptr)
    {
        pre = insert;
        if (newvalue < insert->m_nValue)
            insert = insert->m_pLeft;
        else if (newvalue > insert->m_nValue)
            insert = insert->m_pRight;
    }

    //make link between newnode and pre
    newnode->m_pParent = pre;
    if (newvalue < pre->m_nValue)
        pre->m_pLeft = newnode;
    else if (newvalue > pre->m_nValue)
        pre->m_pRight = newnode;

    m_fFixAfterInsert(newnode);
}

void RbTree::m_fFixAfterInsert(RbTreeNode *node)
{
    RbTreeNode *parent = nullptr, *gparent = nullptr;

    while ((parent = node->m_pParent) != nullptr && parent->m_nColor == red)
    {
        gparent = parent->m_pParent;

        if (parent == gparent->m_pLeft)
        {
            RbTreeNode *uncle = gparent->m_pRight;

            if (uncle != nullptr && uncle->m_nColor == red)
            {
                parent->m_nColor = black;
                uncle->m_nColor = black;
                gparent->m_nColor = red;
                node = gparent;
                continue;
            }

            if (node == parent->m_pRight)
            { //uncle is black && node is parent's right-node
                m_fLeftRotate(parent);
                RbTreeNode *tmp = parent;
                parent = node;
                node = tmp;
            }

            parent->m_nColor = black;
            gparent->m_nColor = red;
            m_fRightRotate(gparent);
        }
        else
        {
            RbTreeNode *uncle = gparent->m_pLeft;

            if (uncle != nullptr && uncle->m_nColor == red)
            {
                parent->m_nColor = black;
                uncle->m_nColor = black;
                gparent->m_nColor = red;
                node = gparent;
                continue;
            }

            if (node == parent->m_pLeft)
            {
                m_fRightRotate(parent);
                RbTreeNode *tmp = parent;
                parent = node;
                node = tmp;
            }

            parent->m_nColor = black;
            gparent->m_nColor = red;
            m_fLeftRotate(gparent);
        }
    }

    m_pRoot->m_nColor = black;
}

void RbTree::m_fDeleteNode(RbTreeNode *node)
{
    RbTreeNode *child = nullptr, *parent = nullptr;
    color color_tmp;

    if ((node->m_pLeft != nullptr) && (node->m_pRight != nullptr))
    {
        RbTreeNode *replace = node;
        replace = replace->m_pRight;
        //find min node
        while (replace->m_pLeft != nullptr)
            replace = replace->m_pLeft;

        if (node->m_pParent != nullptr)
        {
            if (node == node->m_pParent->m_pLeft)
                node->m_pParent->m_pLeft = replace;
            else
                node->m_pParent->m_pRight = replace;
        }
        else
        {
            m_pRoot = replace;
        }

        child = replace->m_pRight; //后继节点肯定不存在左子节点！
        parent = replace->m_pParent;
        color_tmp = replace->m_nColor; //保存后继节点的颜色
        if (parent == node)
        { //后继节点是被删除节点的子节点
            parent = replace;
        }
        else
        {
            if (child != nullptr)
                child->m_pParent = parent;
            parent->m_pLeft = child;
            replace->m_pRight = node->m_pRight;
            node->m_pRight->m_pParent = replace;
        }
        replace->m_pParent = node->m_pParent;
        replace->m_nColor = node->m_nColor; //保持原来位置的颜色
        replace->m_pLeft = node->m_pLeft;
        node->m_pLeft->m_pParent = replace;

        if (color_tmp == black)
        {                                     //4. 如果移走的后继节点颜色是黑色，重新修整红黑树
            m_fFixAfterDelete(child, parent); //将后继节点的child和parent传进去
        }
        node = nullptr;
        return;
    }
}

void RbTree::m_fFixAfterDelete(RbTreeNode *node, RbTreeNode *parent)
{
    RbTreeNode *other = nullptr;

    while ((node == nullptr || node->m_nColor == black) && (node != m_pRoot))
    {
        if (parent->m_pLeft == node)
        {
            other = parent->m_pRight;
            if (other->m_nColor == red)
            { //case1: node的兄弟节点other是红色的
                other->m_nColor = black;
                parent->m_nColor = red;
                m_fLeftRotate(parent);
                other = parent->m_pRight;
            }

            //case2: node的兄弟节点other是黑色的，且other的两个子节点也都是黑色的
            if ((other->m_pLeft == nullptr || other->m_pLeft->m_nColor == black) &&
                    (other->m_pRight == nullptr || other->m_pRight->m_nColor == black))
            {
                other->m_nColor = red;
                node = parent;
                parent = node->m_pParent;
            }
            else
            {
                //case3: node的兄弟节点other是黑色的，且other的左子节点是红色，右子节点是黑色
                if (other->m_pRight == nullptr || other->m_pRight->m_nColor == black)
                {
                    other->m_pLeft->m_nColor = black;
                    other->m_nColor = red;
                    m_fRightRotate(other);
                    other = parent->m_pRight;
                }

                //case4: node的兄弟节点other是黑色的，且other的右子节点是红色，左子节点任意颜色
                other->m_nColor = parent->m_nColor;
                parent->m_nColor = black;
                other->m_pRight->m_nColor = black;
                m_fLeftRotate(parent);
                node = m_pRoot;
                break;
            }
        }
        else
        { //与上面的对称
            other = parent->m_pLeft;

            if (other->m_nColor == red)
            {
                // Case 1: node的兄弟other是红色的
                other->m_nColor = black;
                parent->m_nColor = red;
                m_fRightRotate(parent);
                other = parent->m_pLeft;
            }

            if ((other->m_pLeft == nullptr || other->m_pLeft->m_nColor == black) &&
                    (other->m_pRight == nullptr || other->m_pRight->m_nColor == black))
            {
                // Case 2: node的兄弟other是黑色，且other的俩个子节点都是黑色的
                other->m_nColor = red;
                node = parent;
                parent = node->m_pParent;
            }
            else
            {

                if (other->m_pLeft == nullptr || other->m_pLeft->m_nColor == black)
                {
                    // Case 3: node的兄弟other是黑色的，并且other的左子节点是红色，右子节点为黑色。
                    other->m_pRight->m_nColor = black;
                    other->m_nColor = red;
                    m_fLeftRotate(other);
                    other = parent->m_pLeft;
                }

                // Case 4: node的兄弟other是黑色的；并且other的左子节点是红色的，右子节点任意颜色
                other->m_nColor = parent->m_nColor;
                parent->m_nColor = black;
                other->m_pLeft->m_nColor = black;
                m_fRightRotate(parent);
                node = m_pRoot;
                break;
            }
        }
    }
    if (node != nullptr)
        node->m_nColor = black;
}

// RbTreeNode *RbTree::m_fFindLeftSubTreeMaxNode(RbTreeNode *node)
// {
//     RbTreeNode *leftmax = nullptr, *left = node->getLeft();
//     while (left)
//     {
//         leftmax = left;
//         left = left->getRight();
//     }
//     return leftmax;
// }

// RbTreeNode *RbTree::m_fFindRightSubTreeMinNode(RbTreeNode *node)
// {
//     RbTreeNode *rightmin = nullptr, *right = node->getRight();
//     while (right != nullptr)
//     {
//         rightmin = right;
//         right = right->getLeft();
//     }
//     return rightmin;
// }

void RbTree::m_fDeleteAllNode(RbTreeNode *node, int &deleteNumber)
{
    if (node == nullptr)
        return;
    if (node->m_pLeft != nullptr)
        m_fDeleteAllNode(node->m_pLeft, deleteNumber);
    if (node->m_pRight != nullptr)
        m_fDeleteAllNode(node->m_pRight, deleteNumber);
    if (node->m_pLeft == nullptr && node->m_pRight == nullptr)
    {
        RbTreeNode *parent = node->m_pParent;
        if (parent != nullptr)
        {
            if (node->m_nValue < parent->m_nValue)
                parent->m_pLeft = nullptr;
            else
                parent->m_pRight = nullptr;
        }
        deleteNumber++;
        m_nNumber--;
        delete node;
        node = nullptr;
    }
}

RbTree::RbTree()
{
    m_pRoot = nullptr;
    m_nNumber = 0;
    LOG(Debug) << "class RbTree constructor\n";
}

RbTree::~RbTree()
{
    int deleteNumber = 0;
    int number = getNumber();
    m_fDeleteAllNode(m_pRoot, deleteNumber);
    if (number == deleteNumber && getNumber() == 0)
        std::cout << "all node are deleted\n";
    else
        std::cout << "delete tree failed\n";

    LOG(Debug) << "class RbTree destructor\n";
}

bool RbTree::find(const int &value)
{
    RbTreeNode *node = m_fFindNode(value);
    if (node != nullptr)
        return true;
    else
        return false;
}

void RbTree::insert(const int &value)
{
    RbTreeNode *newnode = new RbTreeNode(value);
    if (m_pRoot == nullptr)
    {
        m_pRoot = newnode;
        m_pRoot->m_nColor = black;
    }
    else
    {
        m_fInsertNode(newnode);
    }
    m_nNumber++;
}

// void RbTree::deleteNode(const int &value)
// {
//     RbTreeNode *node = m_fFindNode(value);
//     if (node != nullptr)
//     {
//         m_fDeleteNode(node);
//         m_nNumber--;
//     }
//     else
//     {
//         std::cout << "don't exists this value\n";
//     }
// }

void RbTree::m_fLeftRotate(RbTreeNode *node)
{
    LOG(Debug) << "leftRotate:" << node->m_nValue << std::endl;

    /* 
     * left rotate function
     *     p                       p 
     *    /                       / 
     *   x                       y 
     *  / \                     / \ 
     * lx  y      ----->       x  ry 
     *    / \                 / \ 
     *   ly ry               lx ly 
     */
    RbTreeNode *x = node;         //must
    RbTreeNode *p = x->m_pParent; //may be
    // RbTreeNode *lx = x->m_pLeft;  //may be
    RbTreeNode *y = x->m_pRight;  //must
    RbTreeNode *ly = y->m_pLeft;  //may be
    // RbTreeNode *ry = y->m_pRight; //may be

    //first step:make link between x and ly
    x->m_pRight = ly;
    if (ly != nullptr)
        ly->m_pParent = x;

    //second step:make link between y and p
    y->m_pParent = p;
    if (p != nullptr)
    {
        if (x == p->m_pLeft)
            p->m_pLeft = y;
        else
            p->m_pRight = y;
    }
    else
    {
        m_pRoot = y;
    }

    //third step:make link between x and y
    y->m_pLeft = x;
    x->m_pParent = y;
}

void RbTree::m_fRightRotate(RbTreeNode *node)
{
    LOG(Debug) << "RightRotate:" << node->m_nValue << std::endl;

    /* 
     * right rotate by node y
     *        p                   p 
     *       /                   / 
     *      y                   x 
     *     / \                 / \ 
     *    x  ry   ----->      lx  y 
     *   / \                     / \ 
     * lx  rx                   rx ry 

*/

    RbTreeNode *y = node;         //must isn't nullptr
    RbTreeNode *p = y->m_pParent; //may be
    RbTreeNode *x = y->m_pLeft;   //must
    // RbTreeNode *lx = x->m_pLeft;  //may be
    RbTreeNode *rx = x->m_pRight; //may be
    // RbTreeNode *ry = y->m_pRight; //may be

    //first step:make link between y and rx
    y->m_pLeft = rx;
    if (rx != nullptr)
        rx->m_pParent = y;

    //second step:make link between x and p
    x->m_pParent = p;
    if (p != nullptr)
    {
        if (y == p->m_pLeft)
            p->m_pLeft = x;
        else
            p->m_pRight = x;
    }
    else
    {
        m_pRoot = x;
    }

    //third step:remake link between x and y
    x->m_pRight = y;
    y->m_pParent = x;
}
