#include "consistent_hash.hpp"
#include <iostream>
#include "hash.hpp"
#include "LibStdCout.h"

using namespace std;
using namespace CONSISTENT_HASH;
using namespace HASH_ALG;

struct Node
{
    Node(): sIp(""), uiPort(0)
    {
    }

    Node(const std::string& ip, uint16_t port):sIp(ip), uiPort(port) 
    {
    }

    Node(const Node& node)
    {
        sIp = node.sIp;
        uiPort = node.uiPort;
    }

    Node& operator = (const Node& node)
    {
        if (&node != this)
        {
            sIp = node.sIp;
            uiPort = node.uiPort;
        }
        return *this;
    }
    
    operator std::string() const
    {
        std::stringstream ios;
        ios << sIp << ":" << uiPort;
        return std::string(ios.str());
    }

    friend std::ostream& operator << (ostream& out, const Node& node)
    {
        out << node.sIp << ":" << node.uiPort;
        return out;
    }
    
    bool operator < (const Node& node) const
    {
        std::ostringstream os;
        os << sIp << ":" << uiPort;
        std::string sSrc = os.str();
        os.str("");
        os << node.sIp << ":" << node.uiPort;
        std::string sDst = os.str();
        return  sSrc < sDst;
    }

    std::string sIp;
    uint16_t uiPort;
};

class CHashFunc
{
 public:
  CHashFunc() {}
  virtual ~CHashFunc() {}
  uint32_t operator() (const std::string& sKey);
};

uint32_t CHashFunc::operator() (const std::string& sKey)
{
    int iRet  = 0;
    iRet = hash_fnv1a_64(sKey.c_str(), sKey.size());
    return iRet;
}


int main()
{
    int iVNodeNums = 3;
    CHasher<Node, CHashFunc, std::string> Hasher(iVNodeNums);
    bool bRet;

    std::string sIp2 = "192.168.1.11"; 
    uint16_t uiPort2 = 4568;
    Node node_two(sIp2, uiPort2);
    
    bRet = Hasher.InsertNode(node_two);
    if (bRet == false)
    {
        ERROR_LOG("insert node into consistent fail, node: %s", std::string(node_two).c_str());
        return 0;
    }
    DEBUG_LOG("insert node into consistent succ, node: %s", std::string(node_two).c_str());

    std::string sIp1 = "192.168.1.10"; 
    uint16_t uiPort1 = 4567;
    Node node_one(sIp1, uiPort1);
    
    bRet = Hasher.InsertNode(node_one);
    if (bRet == false)
    {
        ERROR_LOG("insert node into consistent fail, node: %s", std::string(node_one).c_str());
        return 0;
    }
    DEBUG_LOG("insert node into consistent succ, node: %s", std::string(node_one).c_str());


    std::string sIp3 = "192.168.1.12"; 
    uint16_t uiPort3 = 4569;
    
    Node node_three(sIp3, uiPort3);
    bRet = Hasher.InsertNode(node_three);
    if (bRet == false)
    {
        ERROR_LOG("insert node into consistent fail, node: %s", std::string(node_three).c_str());
        return 0;
    }
    DEBUG_LOG("insert node into consistent succ, node: %s", std::string(node_three).c_str());
    //
    Hasher.IteratorCHasher();
    //
    std::string sKey = "1213131";
    VNode<Node>*  findNode = Hasher.FindNodeByKey(sKey);
    if (findNode == NULL)
    {
        ERROR_LOG("not find consistent node by key: %s", sKey.c_str());
        return 0;
    }
    DEBUG_LOG("get consistent hash node, key: %s, node: %s", sKey.c_str(), std::string(findNode->GetNode()).c_str());

    Hasher.IteratorCHasher();
    
    sKey = "abc";
    findNode = Hasher.FindNodeByKey(sKey);
    if (findNode == NULL)
    {
        std::cout << "not find node" << std::endl;
        return 0;
    }
    DEBUG_LOG("get consistent hash node, key: %s, node: %s", sKey.c_str(), std::string(findNode->GetNode()).c_str());

    Hasher.IteratorCHasher();

    //del node and print key arrangement

    bRet = Hasher.DelNode(node_one);
    if (bRet == false)
    {
        ERROR_LOG("del  node from consistent fail, node: %s", std::string(node_one).c_str());
        return 0;
    }
    DEBUG_LOG("del  node from consistent succ, node: %s", std::string(node_one).c_str());

    Hasher.IteratorCHasher();

    sKey = "abc";
    findNode = Hasher.FindNodeByKey(sKey);
    if (findNode == NULL)
    {
        std::cout << "not find node" << std::endl;
        return 0;
    }
    DEBUG_LOG("get consistent hash node, key: %s, node: %s", sKey.c_str(), std::string(findNode->GetNode()).c_str());

    Hasher.IteratorCHasher();

    return 0;
}


