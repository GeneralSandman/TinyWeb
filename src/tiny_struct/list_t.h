/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---template List class---
 *
 ****************************************
 *
 */

#ifndef LIST_H
#define LIST_H

#include <tiny_base/log.h>

#include <iostream>
#include <vector>

#define AL_START_HEAD 0
#define AL_START_TAIL 1

#define After 0
#define Before 1

template <typename T>
class List;
template <typename T>
class ListIter;

template <typename T>
class ListNode
{
    public:
        ListNode *m_pPrev;
        ListNode *m_pNext;
        T m_nValue;

    public:
        ListNode(const T &v) : m_nValue(v),
        m_pPrev(nullptr),
        m_pNext(nullptr)
    {
        LOG(Debug) << "class ListNode constructor\n";
    }
        ~ListNode()
        {
            LOG(Debug) << "class ListNode destructor\n";
        }
        friend class List<T>;
        friend class ListIter<T>;
};

template <typename T>
class List
{
    public:
        std::string m_nName;
        size_t m_nLen;
        ListNode<T> *m_pHead;
        ListNode<T> *m_pTail;

        bool m_fInsertNode(ListNode<T> *, ListNode<T> *, int);
        bool m_fDeleteNode(ListNode<T> *);
        bool m_fAddNodeHead(ListNode<T> *);
        bool m_fAddNodeTail(ListNode<T> *);
        bool m_fDeleteNodeHead();
        bool m_fDeleteNodeTail();

        ListNode<T> *m_fFindNode(const T &m_nValue);
        ListNode<T> *m_fFindNodeIndex(long index);

    public:
        List(const std::string &);
        std::string getName() { return m_nName; }
        size_t getLen() { return m_nLen; }
        bool insert(T, T, int flag = 0);
        bool dele(T);
        bool add_head(T);
        bool add_tail(T);
        T pop_head();
        T pop_tail();
        bool getByIndex(int, T &);
        bool setByIndex(int, const T &);
        bool getByRange(std::vector<T> &, int, int);
        bool getAll(std::vector<T> &);
        bool clear();

        List dupLict();
        ~List();
};

//The function of ListIter is traversal all
//the ListNode of List,in order to persist
//the list data
template <typename T>
class ListIter
{
    public:
        List<T> *m_pList;
        ListNode<T> *m_pNext;
        int m_nDirction;

    public:
        ListIter(List<T> *list, int di);
        void listRewindHead();
        void listRewindTail();
        ListNode<T> *getListNext();
        ~ListIter()
        {
            m_pList = nullptr;
            m_pNext = nullptr;
        }
};

//---List-api--------//
    template <typename T>
List<T>::List(const std::string &name)
{
    m_nName = name;
    m_nLen = 0;
    m_pHead = nullptr;
    m_pTail = nullptr;
    LOG(Debug) << "class List constructor\n";
}

    template <typename T>
bool List<T>::m_fInsertNode(ListNode<T> *newnode, ListNode<T> *oldnode, int flag)
{

    if (After == flag)
    {
        ListNode<T> *oldnext = oldnode->m_pNext;
        newnode->m_pNext = oldnext;
        newnode->m_pPrev = oldnode;
        oldnode->m_pNext = newnode;
        if (oldnext)
            oldnext->m_pPrev = newnode;
        if (m_pTail == oldnode)
            m_pTail = newnode;
    }
    else if (Before == flag)
    {
        ListNode<T> *oldprev = oldnode->m_pPrev;
        newnode->m_pPrev = oldprev;
        newnode->m_pNext = oldnode;
        oldnode->m_pPrev = newnode;
        if (oldprev)
            oldprev->m_pNext = newnode;
        if (m_pHead == oldnode)
            m_pHead = newnode;
    }

    m_nLen++;
    return true;
}

    template <typename T>
bool List<T>::m_fDeleteNode(ListNode<T> *node)
{

    if (node->m_pPrev)
        node->m_pPrev->m_pNext = node->m_pNext;
    else //delete head
        m_pHead = node->m_pNext;

    if (node->m_pNext)
        node->m_pNext->m_pPrev = node->m_pPrev;
    else //delete tail
        m_pTail = node->m_pPrev;

    m_nLen--;
    delete node;
    node = nullptr;
    return true;
}

    template <typename T>
bool List<T>::m_fAddNodeHead(ListNode<T> *node)
{

    if (getLen() == 0)
    {
        m_pHead = m_pTail = node;
        node->m_pPrev = nullptr;
        node->m_pNext = nullptr;
    }
    else
    {
        node->m_pPrev = nullptr;
        m_pHead->m_pPrev = node;
        node->m_pNext = m_pHead;
        m_pHead = node;
    }
    m_nLen++;
    return true;
}

    template <typename T>
bool List<T>::m_fAddNodeTail(ListNode<T> *node)
{
    if (getLen() == 0)
    {
        m_pHead = m_pTail = node;
        node->m_pPrev = node->m_pNext = nullptr;
    }
    else
    {
        node->m_pNext = nullptr;
        m_pTail->m_pNext = node;
        node->m_pPrev = m_pTail;
        m_pTail = node;
    }
    m_nLen++;
    return true;
}

    template <typename T>
bool List<T>::m_fDeleteNodeHead()
{
    if (m_nLen == 0)
        return false;
    m_fDeleteNode(m_pHead);
}

    template <typename T>
bool List<T>::m_fDeleteNodeTail()
{
    if (m_nLen == 0)
        return false;
    m_fDeleteNode(m_pTail);
}

    template <typename T>
List<T> List<T>::dupLict()
{
}

    template <typename T>
ListNode<T> *List<T>::m_fFindNode(const T &v)
{
    ListNode<T> *res;
    res = m_pHead;
    while (res != nullptr)
    {
        if (res->m_nValue == v)
            break;
        else
        {
            res = res->m_pNext;
        }
    }

    return res;
}

    template <typename T>
ListNode<T> *List<T>::m_fFindNodeIndex(long index)
{
    ListNode<T> *res = nullptr;
    if (index >= 0)
    {
        if (index >= m_nLen)
            return nullptr;
        res = m_pHead;
        while (index)
        {
            index--;
            res = res->m_pNext;
        }
    }
    else
    {
        index = -index - 1;
        if (index >= m_nLen)
            return nullptr;
        res = m_pTail;
        while (index)
        {
            index--;
            res = res->m_pPrev;
        }
    }

    return res;
}

    template <typename T>
bool List<T>::insert(T newvalue, T oldvalue, int flag)
{

    ListNode<T> *oldnode = m_fFindNode(oldvalue);
    if (oldnode == nullptr)
        return false;

    ListNode<T> *newnode =
        new ListNode<T>(newvalue);
    return m_fInsertNode(newnode, oldnode, flag);
}

    template <typename T>
bool List<T>::dele(T value)
{
    ListNode<T> *node = m_fFindNode(value);
    return m_fDeleteNode(node);
}

    template <typename T>
bool List<T>::add_head(T value)
{
    ListNode<T> *newnode =
        new ListNode<T>(value);
    return m_fAddNodeHead(newnode);
}

    template <typename T>
bool List<T>::add_tail(T value)
{
    ListNode<T> *newnode =
        new ListNode<T>(value);
    return m_fAddNodeTail(newnode);
}

    template <typename T>
T List<T>::pop_head()
{
    if (m_nLen)
    {
        T res = m_pHead->m_nValue;
        m_fDeleteNodeHead();
        return res;
    }
}

    template <typename T>
T List<T>::pop_tail()
{
    if (m_nLen)
    {
        T res = m_pTail->m_nValue;
        m_fDeleteNodeTail();
        return res;
    }
}
    template <typename T>
bool List<T>::getByIndex(int index, T &value)
{
    if (index >= m_nLen)
    {
        return false;
    }
    else
    {
        ListIter<T> iter(this, AL_START_HEAD);
        ListNode<T> *next = nullptr;
        int i = 0;

        while ((next = iter.getListNext()) != nullptr)
        {
            if (i == index)
            {
                value = next->m_nValue;
                break;
            }
            i++;
        }
        return true;
    }
}

    template <typename T>
bool List<T>::setByIndex(int index, const T &value)
{
    if (index >= m_nLen)
    {
        return false;
    }
    else
    {
        ListIter<T> iter(this, AL_START_HEAD);
        ListNode<T> *next = nullptr;
        int i = 0;

        while ((next = iter.getListNext()) != nullptr)
        {
            if (i == index)
            {
                next->m_nValue = value;
                break;
            }
            i++;
        }
        return true;
    }
}

    template <typename T>
bool List<T>::getByRange(std::vector<T> &res, int begin, int end)
{
    if (begin > end || begin > m_nLen || end > m_nLen)
        return false;

    int size = end - begin;
    res.resize(size);

    ListIter<T> iter(this, AL_START_HEAD);
    ListNode<T> *next = nullptr;

    int vector_index = 0;
    int list_index = 0;

    while ((next = iter.getListNext()) != nullptr)
    {
        if (list_index < begin)
        {
            list_index++;
        }
        else if (begin <= list_index && list_index < end)
        {
            res[vector_index] = next->m_nValue;
            vector_index++;
            list_index++;
        }
        else if (list_index >= end)
        {
            break;
        }
    }
    return true;
}

    template <typename T>
bool List<T>::getAll(std::vector<T> &res)
{
    getByRange(res, 0, m_nLen);
    return true;
}

    template <typename T>
bool List<T>::clear()
{
    while (m_nLen)
    {
        m_fDeleteNodeHead();
    }
    if (!m_nLen)
        return true;
    else
        return false;
}

    template <typename T>
List<T>::~List()
{
    size_t len = getLen();
    ListNode<T> *cur = m_pHead, *next = nullptr;

    while (len)
    {
        len--;
        next = cur->m_pNext;
        delete cur;
        cur = next;
    }
    LOG(Debug) << "class List destructor\n";
}
//---List-api-end----//

//--ListIter api-----//
    template <typename T>
ListIter<T>::ListIter(List<T> *list, int di)
{
    m_pList = list;
    m_nDirction = di;
    if (di == AL_START_HEAD)
        m_pNext = list->m_pHead;
    else
        m_pNext = list->m_pTail;
}

    template <typename T>
void ListIter<T>::listRewindHead()
{
    m_pNext = m_pList->m_pHead;
    m_nDirction = AL_START_HEAD;
}

    template <typename T>
void ListIter<T>::listRewindTail()
{
    m_pNext = m_pList->m_pTail;
    m_nDirction = AL_START_TAIL;
}

    template <typename T>
ListNode<T> *ListIter<T>::getListNext()
{
    ListNode<T> *res = m_pNext;

    if (res != nullptr)
    {
        if (m_nDirction == AL_START_HEAD)
            m_pNext = res->m_pNext;
        else
            m_pNext = res->m_pPrev;
    }

    return res;
}

//--ListIter api-end-//

#endif //DLIST_H
