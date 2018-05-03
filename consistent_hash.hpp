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
      void SetVNodeIndex(uint32_t index);
      void IncrKeyNums();
      void SetHashVal(const uint32_t uiHash);
      T& GetNode()
      {
          return m_Node;
      }
      int32_t GetVNodeIndex() // node hash index
      {
          return m_iVNodeIndex;
      }
      int32_t GetKeyNums()
      {
          return m_iKeyNums;
      }

     private:
      T m_Node; //vnode content.
      int32_t m_iVNodeNums; //node has vnode nums
      uint32_t  m_iVNodeIndex; //cur vnode index in list of vnode list
      int32_t m_iKeyNums; //this vnode has nums of key in hash ring.
      uint32_t m_uiHashVal; //this vnode hash val
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
