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

#ifndef DICT_H
#define DICT_H

#include <tiny_base/log.h>

#include <string>
#include <cstring>
#include <deque>
#include <iostream>

    template <typename K>
unsigned int hashFunction(const K &key)
{
    std::hash<std::string> h;
    unsigned int n = h(key);
    return n;
}

template <typename K, typename V>
class Dict;
template <typename K, typename V>
class DictTable;
template <typename K, typename V>
class DictIter;

template <typename K, typename V>
class DictEntry
{
    private:
        K m_nKey;
        V m_nValue;
        DictEntry *m_pNext;

    public:
        DictEntry() : m_pNext(nullptr)
    {
        LOG(Debug) << "class DictEntry constructor" << std::endl;
    }
        K getKey() { return m_nKey; }
        V getValue() { return m_nValue; }
        ~DictEntry()
        {
            m_pNext = nullptr;
            LOG(Debug) << "class DictEntry destructor" << std::endl;
        }

        friend class Dict<K, V>;
        friend class DictTable<K, V>;
        friend class DictIter<K, V>;
};

template <typename K, typename V>
class DictTable
{
    private:
        DictEntry<K, V> **m_pTable;
        size_t m_nSize;
        size_t m_nSizeMask;
        size_t m_nUsed;

        unsigned int m_fGetIndex(const K &);
        DictEntry<K, V> *m_fGetEntry(const K &);
        DictEntry<K, V> *m_fFindEntry(const K &);

    public:
        DictTable(size_t size = 4);
        bool insertPair(const K &, const V &);
        bool findPair(const K &, V &);
        bool deletePair(const K &);
        bool clearAll();
        V operator[](const K &);
        ~DictTable();

        friend class Dict<K, V>;
        friend class DictIter<K, V>;
};

template <typename K>
class DictFunctor
{
    unsigned int m_fHashFunction(const K &);
    //dup,compare
};

template <typename K, typename V>
class Dict
{
    private:
        std::string m_nDictName;
        DictFunctor<K> *m_pFunctor;
        DictTable<K, V> *m_pTables[2];
        int m_nReHash;

    public:
        Dict(const std::string &name) : m_nDictName(name),
        m_pFunctor(nullptr),
        m_nReHash(0)
    {
        m_pTables[0] = new DictTable<K, V>(4);
        m_pTables[1] = new DictTable<K, V>(4);
        LOG(Debug) << "class Dict constructor\n";
    };

        std::string getName() { return m_nDictName; }
        bool dictSet(const K &key, const V value)
        {
            if (!m_nReHash)
            {
                return m_pTables[0]->insertPair(key, value);
            }
            else
            {
                return m_pTables[1]->insertPair(key, value);
            }
        }
        bool dictGet(const K &key, V &value)
        {
            //have bug
            if (!m_nReHash)
            {
                return m_pTables[0]->findPair(key, value);
            }
            else
            {
                return m_pTables[1]->findPair(key, value);
            }
        }
        int dictLen()
        {
            if (!m_nReHash)
                return m_pTables[0]->m_nUsed;
            else
                return m_pTables[0]->m_nUsed + m_pTables[1]->m_nUsed;
        }

        bool dictClear()
        {
            m_pTables[0]->clearAll();
            m_pTables[1]->clearAll();
            return true;
        }

        ~Dict()
        {
            delete m_pTables[0];
            delete m_pTables[1];
            LOG(Debug) << "class Dict destructor\n";
        }

        friend class DictIter<K, V>;
};

//The function of ListIter is traversal all
//the ListNode of List,in order to persist
//the list data
template <typename K, typename V>
class DictIter
{
    private:
        Dict<K, V> *m_pDict;
        DictEntry<K, V> *m_pNext_;
        int m_nNoTraver;

        int m_nTab;   //table number
        int m_nIndex; //index number

        DictEntry<K, V> *m_fFindNextEntry()
        {
            bool find = false;
            int table_size = m_pDict->m_pTables[0]->m_nSize;

            for (; m_nTab < 2; m_nTab++)
            {
                for (; m_nIndex < table_size; m_nIndex++)
                {
                    if (*(m_pDict->m_pTables[m_nTab]->m_pTable + m_nIndex) != nullptr)
                    {
                        find = true;
                        break;
                    }
                }

                if (find)
                    break;
            }

            if (find)
                return *(m_pDict->m_pTables[m_nTab]->m_pTable + m_nIndex);
            else
                return nullptr;
        }

    public:
        DictIter(Dict<K, V> *dict) : m_pDict(dict),
        m_pNext_(nullptr),
        m_nNoTraver(dict->dictLen()),
        m_nTab(0),
        m_nIndex(0)
    {
        m_pNext_ = m_fFindNextEntry();
        // reset2Begin();
    }

        void reset2Begin()
        {
            m_nNoTraver = m_pDict->dictLen();
            m_nTab = 0;
            m_nIndex = 0;
            m_pNext_ = m_fFindNextEntry();
        }
        DictEntry<K, V> *getDictNext()
        {
            if (!m_nNoTraver)
                return nullptr;

            //There are Entrys haven't been traveraled;
            m_nNoTraver--;
            DictEntry<K, V> *res = m_pNext_;

            bool findNext = false;
            if (m_pNext_ != nullptr)
            {
                if (m_pNext_->m_pNext != nullptr)
                {
                    m_pNext_ = m_pNext_->m_pNext;
                }
                else
                {
                    findNext = true;
                    m_nIndex++;
                }
            }

            if (findNext)
                m_pNext_ = m_fFindNextEntry();

            return res;
        }
        ~DictIter()
        {
            m_pDict = nullptr;
            m_pNext_ = nullptr;
        }
};

//........api.............//

    template <typename K, typename V>
DictTable<K, V>::DictTable(size_t size)
{
    m_nSize = size;
    m_nSizeMask = size - 1;
    m_nUsed = 0;
    m_pTable = new DictEntry<K, V> *[size];
    for (int i = 0; i < size; i++)
        m_pTable[i] = nullptr;

    LOG(Debug) << "class DictTable constructor\n";
}

    template <typename K, typename V>
unsigned int DictTable<K, V>::m_fGetIndex(const K &k)
{
    unsigned int hash = hashFunction(k);
    unsigned int index = hash % m_nSize;
    return index;
}

    template <typename K, typename V>
DictEntry<K, V> *DictTable<K, V>::m_fFindEntry(const K &k)
{
    //if exists return a pointer,else return nullptr;
    unsigned int index = m_fGetIndex(k);
    if (m_pTable[index] == nullptr)
        return nullptr;
    DictEntry<K, V> *cur = m_pTable[index];
    while (cur != nullptr)
    {
        if (cur->m_nKey == k)
            return cur;
        else
        {
            cur = cur->m_pNext;
        }
    }
    return nullptr;
}

    template <typename K, typename V>
DictEntry<K, V> *DictTable<K, V>::m_fGetEntry(const K &k)
{
    //if exists return a pointer,else create a new Entry
    DictEntry<K, V> *find = m_fFindEntry(k);
    if (find == nullptr)
    {
        unsigned int index = m_fGetIndex(k);
        find = new DictEntry<K, V>();
        find->m_nKey = k;
        find->m_pNext = m_pTable[index];
        m_pTable[index] = find;
        m_nUsed++;
    }
    return find;
}

    template <typename K, typename V>
bool DictTable<K, V>::insertPair(const K &k, const V &v)
{
    DictEntry<K, V> *get = m_fGetEntry(k);
    get->m_nValue = v;
    return true;
}

    template <typename K, typename V>
bool DictTable<K, V>::findPair(const K &k, V &v)
{
    DictEntry<K, V> *find = m_fFindEntry(k);
    if (find == nullptr)
    {
        return false;
    }
    else if (find->m_nKey == k)
        v = find->m_nValue;
}

    template <typename K, typename V>
V DictTable<K, V>::operator[](const K &k)
{
    V res;
    findPair(k, res);
    return res;
}

    template <typename K, typename V>
bool DictTable<K, V>::deletePair(const K &k)
{
    //delete
    //don't finish
    return true;
}

    template <typename K, typename V>
bool DictTable<K, V>::clearAll()
{
    int tmp = 0;
    for (int i = 0; i < m_nSize; i++)
    {
        if (m_pTable[i])
        {
            DictEntry<K, V> *cur = m_pTable[i], *pre = cur;
            while (pre)
            {
                cur = cur->m_pNext;
                tmp++;
                delete pre;
                pre = cur;
            }
        }

        m_pTable[i] = nullptr;
    }

    if (m_nUsed == tmp)
    {
        m_nUsed = 0;
        return true;
    }
}

    template <typename K, typename V>
DictTable<K, V>::~DictTable()
{
    clearAll();
    delete[] m_pTable;
    m_pTable = nullptr;
    m_nSize = 0;
    m_nSizeMask = 0;
    m_nUsed = 0;
    LOG(Debug) << "class DictTable destructor\n";
}

#endif
