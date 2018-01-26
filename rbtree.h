/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/sigil
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
****************************************
*
*/

#ifndef Rb_TREE
#define Rb_TREE

#include "log.h"

#include <iostream>
#include <vector>
using namespace std;

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

  void m_fLevel(vector<vector<entry>> &result, RbTreeNode *node, int level)
  {
    if (node == nullptr)
      return;
    if (level == result.size())
      result.push_back(vector<entry>());
    result[level].push_back(entry(node->m_nValue, node->m_nColor));
    m_fLevel(result, node->m_pLeft, level + 1);
    m_fLevel(result, node->m_pRight, level + 1);
  }
  void PrintTreeLevel()
  {
    vector<vector<entry>> result;
    m_fLevel(result, m_pRoot, 0);
    if (result.empty())
      cout << "empty\n";
    for (int i = 0; i < result.size(); i++)
    {
      cout << "level " << i + 1 << ":";
      for (auto t : result[i])
      {
        char c;
        if (t.second == red)
          c = 'R';
        if (t.second == black)
          c = 'B';

        cout << t.first << c << " ";
      }
      cout << endl;
    }
  }
};

#endif