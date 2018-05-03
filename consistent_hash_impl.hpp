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
void VNode<T>::SetVNodeIndex(uint32_t index)
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
CHasher<T,H, K>::CHasher(uint32_t iVNodeNums ): m_uiVNodeNums(iVNodeNums)
{
    if (m_uiVNodeNums < 1)
    {
        m_uiVNodeNums = 1;
    }
    m_uiHashMask = uint32_t(1 << 32) - 1;
}

template<typename T, typename H, typename K>
CHasher<T,H, K>::~CHasher()
{
    typename std::map<uint32_t, VNode<T>*>::iterator it;
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
bool CHasher<T,H, K>::InsertVNode(const T& tNode, int iVNodeIndex)
{
    VNode<T>* pNode = new VNode<T>(tNode, m_uiVNodeNums);
    if (pNode == NULL)
    {
        return false;
    }

    std::ostringstream os;
    os << pNode->GetNode() << ":" << iVNodeIndex;
    uint32_t uiHash = m_Hasher(std::string(os.str()));
    pNode->SetHashVal(uiHash);

    uint32_t uiHashIndex =  uiHash & m_uiHashMask;
    pNode->SetVNodeIndex(iVNodeIndex);

    typename std::pair<typename std::map<uint32_t, VNode<T>*>::iterator, bool> retInsert;
    retInsert = m_mpCHNodes.insert(std::pair<uint32_t, VNode<T>*>(uiHashIndex, pNode));
    if (retInsert.second == false)
    {
        delete pNode;
        return false;
    }

    return true;
}

template<typename T, typename H, typename K>
void  CHasher<T,H, K>::UpdateCHIndex()
{
    uint32_t uiIndex = 0;
    m_mpIndexVnode.clear();

    for (typename std::map<uint32_t, VNode<T>*>::iterator it = m_mpCHNodes.begin(); it != m_mpCHNodes.end(); ++it)
    {
        m_mpIndexVnode[uiIndex] = it->second;
        uiIndex++;
    }
}


template<typename T, typename H, typename K>
bool CHasher<T,H, K>::InsertNode(const T& tNode)
{
    VNode<T>* pNode = new VNode<T>(tNode, m_uiVNodeNums);
    if (pNode == NULL)
    {
        return false;
    }
    //
    for (uint32_t i = 0; i < m_uiVNodeNums; ++i)
    {
        if ( false == InsertVNode(tNode, i) )
        {
            return false;
        }
    }

    UpdateCHIndex();
    return true;
}

template<typename T, typename H, typename K>
bool CHasher<T,H, K>::DelVNode(const T& tNode, int iVNodeIndex)
{
    std::ostringstream os;
    os << tNode << ":" << iVNodeIndex;

    uint32_t uiHash = m_Hasher(std::string(os.str()));
    uint32_t uiHashIndex =  uiHash & m_uiHashMask;

    typename std::map<uint32_t, VNode<T>*>::iterator it;
    it = m_mpCHNodes.find(uiHashIndex);
    if (it == m_mpCHNodes.end())
    {
        return false;
    }
    if (it->second)
    {
        delete it->second;
    }

    m_mpCHNodes.erase(it);
    //
    return true;
}

template<typename T, typename H, typename K>
bool CHasher<T,H, K>::DelNode(const T& tNode)
{
    for ( uint32_t i = 0;  i < m_uiVNodeNums; ++i )
    {
        DelVNode(tNode, i);
    }
    UpdateCHIndex();
    return true;
}


template<typename T, typename H, typename K>
void CHasher<T,H, K>::IteratorCHasher()
{
    //vnode list for one node.
    std::map<T, std::vector<VNode<T>*> > tmpNodeList;
    
    typename std::map<uint32_t, VNode<T>*>::iterator it;
    for (it = m_mpCHNodes.begin(); it != m_mpCHNodes.end(); ++it)
    {
        if (it->second)
        {
            tmpNodeList[it->second->GetNode()].push_back(it->second);
        }
    }

    typename std::map<T, std::vector<VNode<T>*> >::iterator itVnode;
    for (itVnode = tmpNodeList.begin(); itVnode != tmpNodeList.end(); ++itVnode)
    {
        std::ostringstream os;

        os << "one Node: " << itVnode->first << ", vnode list: [ ";
        for (typename std::vector<VNode<T>*>::iterator itVnodeList = itVnode->second.begin();
             itVnodeList != itVnode->second.end(); ++ itVnodeList)
        {
            os << "vnodeIndx: " << (*itVnodeList)->GetVNodeIndex() << ", keynums: " 
                << (*itVnodeList)->GetKeyNums() <<"; ";
        }
            
        os << " ] ";
        std::cout << os.str() << std::endl;
    }
}

template<typename T, typename H, typename K>
VNode<T>* CHasher<T,H, K>::FindNodeByKey(const K& inKey)
{
    if (m_mpCHNodes.empty())
    {
        return NULL;
    }

    VNode<T>* vNodeRet = NULL;
    std::ostringstream os;
    os << inKey;

    uint32_t uiHash = m_Hasher(std::string(os.str()));
    uint32_t uiPoint =  uiHash & m_uiHashMask;

    uint32_t uiMid = 0, uiLo = 0, uiH = m_mpCHNodes.size() - 1;
    while(1)
    {
        typename std::map<uint32_t, VNode<T>*>::iterator itLo = m_mpIndexVnode.find(uiLo);
        if (itLo == m_mpIndexVnode.end())
        {
            //cout
            break;
        }
        typename std::map<uint32_t, VNode<T>*>::iterator itHo = m_mpIndexVnode.find(uiH);
        if (itHo == m_mpIndexVnode.end())
        {
            //cout
            break;
        }

        if (uiPoint <= itLo->second->GetVNodeIndex()  || 
            uiPoint > itHo->second->GetVNodeIndex() )
        {
            //cout 
            return itLo->second;
        }
        
       
        uiMid = uiLo + (uiH - uiLo)/2;

        typename std::map<uint32_t, VNode<T>*>::iterator itMid = m_mpIndexVnode.find(uiMid);
        if (itMid == m_mpIndexVnode.end())
        {
            //cout 
            break;
        }

        if ( uiPoint <=  itMid->second->GetVNodeIndex() &&
            uiPoint > (uiMid ?  m_mpIndexVnode[uiMid-1]->GetVNodeIndex(): 0) )
        {
            //cout
            return m_mpIndexVnode[uiMid];
        }

        if (itMid->second->GetVNodeIndex() < uiPoint)
        {
            uiLo = uiMid + 1;
        }
        else 
        {
            uiH = uiMid -1;
        }
    }
    return vNodeRet;
}

///
}
#endif
