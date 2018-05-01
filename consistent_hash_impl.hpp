#ifndef _consistent_hash_impl_hpp_
#define _consistent_hash_impl_hpp_

#include "consistent_hash.hpp"

namespace CONSISTENT_HASH
{

template<typename T>
VNode<T>::VNode(const T& tNode, int iVNodeNums): m_Node(tNode), m_iVNodeNums(iVNodeNums), 
    m_iVNodeIndex(-1), m_iKeyNums(0), m_uiHashVal(0)
{
}

template<typename T>
VNode<T>::~VNode()
{
    m_iVNodeNums = 0;
    m_iVNodeIndex = -1;
    m_iKeyNums = 0;
    m_uiHashVal = 0;
}

template<typename T>
void VNode<T>::IncrKeyNums()
{
    m_iKeyNums ++;
}

template<typename T>
void VNode<T>::SetVNodeIndex(int32_t index)
{
    m_iVNodeIndex = index;
}

template<typename T>
void VNode<T>::SetHashVal(uint32_t uiHashVal)
{
    m_uiHashVal = uiHashVal;
}

//---------------------------------------------------------------------------//
template<typename T, typename H, typename K>
CHasher<T,H, K>::CHasher()
{
}

template<typename T, typename H, typename K>
CHasher<T,H, K>::~CHasher()
{
    std::map<uint32_t, VNode<T>*>::iterator it ;
    for(it = m_mpCHNodes.begin(); it != m_mpCHNodes.end(); ++it)
    {
        if (it->second)
        {
            delete (it->second);
        }
    }
    m_mpCHNodes.clear();
}

template<typename T, typename H, typename K>
bool CHasher<T,H, K>::CHasher<T,H, K>::InsertNode(const T& tNode, int32_t iVNodeNums)
{

    return true;
}

template<typename T, typename H, typename K>
bool CHasher<T,H, K>::DelNode(const T& tNode, int32_t iVNodeNums)
{

    return true;
}

template<typename T, typename H, typename K>
VNode<T>* CHasher<T,H, K>::FindNodeByKey(const K& inKey)
{
    VNode<T>* vNodeRet = NULL;
    
    return vNodeRet;
}

template<typename T, typename H, typename K>
void CHasher<T,H, K>::IteratorCHasher()
{

}


///
}
#endif
