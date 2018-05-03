#include "consistent_hash.hpp"
#include <iostream>
#include "hash.hpp"

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
    int iVNodeNums = 1;
    CHasher<Node, CHashFunc, std::string> Hasher(iVNodeNums);

    std::string sIp1 = "192.168.1.10"; 
    uint16_t uiPort1 = 4567;
    Node node_one(sIp1, uiPort1);
    bool bRet = Hasher.InsertNode(node_one);
    if (bRet == false)
    {
        //fail
        return 0;
    }

    std::string sIp2 = "192.168.1.11"; 
    uint16_t uiPort2 = 4568;
    Node node_two(sIp2, uiPort2);
    bRet = Hasher.InsertNode(node_two);
    if (bRet == false)
    {
        //err
        return 0;
    }

    std::string sIp3 = "192.168.1.12"; 
    uint16_t uiPort3 = 4569;
    Node node_three(sIp3, uiPort3);
    bRet = Hasher.InsertNode(node_three);
    if (bRet == false)
    {
        //err
        return 0;
    }
    //
    //Hasher.IteratorCHasher();
    //
    std::string sKey = "1213131";
    VNode<Node>*  findNode = Hasher.FindNodeByKey(sKey);
    if (findNode == NULL)
    {
        std::cout << "not find node" << std::endl;
        return 0;
    }
    std::cout << "find skey: " << sKey << ", node" << std::endl;
    std::cout << findNode->GetNode() << std::endl;

    sKey = "abc";
    findNode = Hasher.FindNodeByKey(sKey);
    if (findNode == NULL)
    {
        std::cout << "not find node" << std::endl;
        return 0;
    }
    std::cout << "find skey: " << sKey << ", node" << std::endl;
    std::cout << findNode->GetNode() << std::endl;

    return 0;
}


