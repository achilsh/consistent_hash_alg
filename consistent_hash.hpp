#ifndef  _CONSISTENT_HASH_HPP_
#define  _CONSISTENT_HASH_HPP_

#include <stdint.h>
#include "LibStdCout.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

namespace CONSISTENT_HASH
{
    //T support to_string() ability. 
    template<typename T> //T may struct of {ip, port} or others.
    class VNode 
    {
     public:
      VNode(const T& tNode, int iVNodeNums);
      virtual ~VNode();
      void SetVNodeHashIndex(uint32_t index);
      void SetVNodeSeq(int32_t iseq)
      {
          m_iVnodeSeq = iseq;
      }
      int32_t GetVNodeSeq()
      {
          return m_iVnodeSeq;
      }
      void IncrKeyNums();
      void SetHashVal(const uint32_t uiHash);
      T& GetNode()
      {
          return m_Node;
      }
      uint32_t GetVNodeHashIndex() // node hash index in vnode list.
      {
          return m_iVNodeHashIndex;
      }
      int32_t GetKeyNums()
      {
          return m_iKeyNums;
      }
      uint32_t GetVnodeHashVal()
      {
          return m_uiHashVal;
      }

     private:
      T m_Node; //vnode content.
      int32_t m_iVNodeNums; //node has vnode nums
      uint32_t  m_iVNodeHashIndex; //cur vnode hash index in ring vnode list, not seq .
      int32_t m_iKeyNums; //this vnode has nums of key in hash ring.
      uint32_t m_uiHashVal; //this vnode hash val
      int32_t m_iVnodeSeq; //cur vnode in node seq node.

    };

    //------------------------------------------//
    //T support << operator, < operator 
    //H support one param,and type is std::string or char*, ret type uint32_t
    //k support  << operator  
    template<typename T, typename H, typename K>
    class CHasher
    {
     public:
      CHasher(uint32_t iVNodeNums = 1);
      virtual ~CHasher();

      bool InsertNode(const T& tNode);
      bool DelNode(const T& tNode);
      VNode<T>* FindNodeByKey(const K& inKey);
      void IteratorCHasher();

     private: 
      bool InsertVNode(const T& tNode, int iVNodeIndex);
      bool DelVNode(const T& tNode, int iVNodeIndex);
      void UpdateCHIndex();

     private:
      std::map<uint32_t, VNode<T>*> m_mpCHNodes; //key: hash_index, 
      std::map<uint32_t, VNode<T>*> m_mpIndexVnode; //key: index => [0 - (n-1)]
      H m_Hasher;
      uint32_t m_uiVNodeNums;
      uint32_t m_uiHashMask;
    };
}
#endif 

#include "consistent_hash_impl.hpp"
