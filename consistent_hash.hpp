#ifndef  _CONSISTENT_HASH_HPP_
#define  _CONSISTENT_HASH_HPP_

#include <stdint.h>
namespace CONSISTENT_HASH
{

    template<typename T>
    class VNode 
    {
     public:
      VNode(const T& tNode, int iVNodeNums);
      virtual ~VNode();
      void SetVNodeIndex(int32_t index);
      void IncrKeyNums();
      void SetHashVal();
     private:
      T m_Node;
      int32_t m_iVNodeNums;
      int32_t m_iVNodeIndex;
      int32_t m_iKeyNums;
      uint32_t m_uiHashVal;
    };

    //------------------------------------------//
    template<typename T, typename H, typename K>
    class CHasher
    {
     public:
      CHasher();
      virtual ~CHasher();
      
      bool InsertNode(const T& tNode, int32_t iVNodeNums);
      bool DelNode(const T& tNode, int32_t iVNodeNums);
     
      VNode<T>* FindNodeByKey(const K& inKey);

      void IteratorCHasher();

     private:
      std::map<uint32_t, VNode<T>*> m_mpCHNodes;
      H m_hasher;
    };



}
#endif 

#include "consistent_hash_impl.hpp"
