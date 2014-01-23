#include "iostream"
#include "ns3/internet-module.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "queue"

#ifndef NODE_BUFFER_H
#define NODE_BUFFER_H
namespace ns3{
class lteEpcPacket{
    public:
        Ptr<Packet> m_packet;
        Ipv4Address m_ipadd;
};
class lteEpcBuffer{
    public:

//        static TypeId GetTypeId(void);

        int getSize();

        int maxSize();

        lteEpcPacket getPacket();

        void push(lteEpcPacket);

        std::queue<lteEpcPacket > m_queue;

        int m_maxsize;
};
}
#endif
