#include "consistent_hash.hpp"
#include <iostream>


using namespace std;
using namespace CONSISTENT_HASH;

struct Node
{
    Node(): sIp(""), uiPort(0)
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

    std::string sIp;
    uint32_t uiPort;
};

class CHashFunc
{
 public:
  CHashFunc() {}
  virtual ~CHashFunc() {}
  uint32_t operator() (const VNode<struct Node>& vNode);
  uint32_t operator() (const std::string& sKey);
};

uint32_t CHashFunc::operator() (const VNode<struct Node>& vNode)
{
    int iRet  = 0;
    return iRet;
}

uint32_t CHashFunc::operator() (const std::string& sKey)
{
    int iRet  = 0;
    return iRet;
}


int main()
{


    return 0;
}


