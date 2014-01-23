#include <iostream>
#include <string>
#include "vector"
#include "queue"
//#include <ns3/socket>
#include <ns3/csma-module.h>
#include <ns3/internet-module.h>
#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/system-mutex.h>
#include <ns3/packet.h>
#include "ns3/ptr.h"
#include "ns3/applications-module.h"
#include "ns3/system-thread.h"
#include <unistd.h>
/*background : four switch nodes send packets to control node
 * purpose : use it to test buffer and thread
 */
using namespace ns3;

#define threadNum  6                //thread Num
int usleepTime = 1500;              //the time process to packet (us);
int maxBuffSize = 100;              //buf size
int defineTotalPacketNum = 100000;  //the total num of send packets
class buffer{
    public:
        buffer();

        int getSize();
        int maxSize();
        Ptr<Packet> getPacket();
        void push(Ptr<Packet>);
        std::vector<Ptr<Packet> > m_vec;
        std::queue< Ptr<Packet> >m_queue;
        int m_maxSize;
};
buffer::buffer(){
    m_maxSize = maxBuffSize;//buffer size
}
int buffer::getSize(){
    //return m_vec.size();
    return m_queue.size();
}
int buffer::maxSize(){
    return m_maxSize;
}
Ptr<Packet> buffer::getPacket(){
 //  Ptr<Packet> packet = *m_vec.begin();
 //  m_vec.erase(m_vec.begin()); 
//    Ptr<Packet> packet = *m_vec.end();
//    m_vec.erase(m_vec.end()-1); 

   Ptr<Packet> packet = m_queue.front();
   m_queue.pop();
   return packet;
}
void buffer::push(Ptr<Packet> packet){
    m_queue.push(packet);
}
buffer buf;
int sendPacketNum = 0;  //use to count the send packets
int lossPacketNum = 0;  //use to count the loss packets
int dealPacketNum = 0;  //use to count the processed packets , the sum of loss packets and dealpackets is the totalSendPacket
int in = 0;
float dpkt = 0.1;
SystemMutex m_mutex;
SystemMutex m_cond;

//receive function
void RecvFunc(Ptr<Socket> socket){
    Ptr<Packet> packet = socket->Recv(); 
    m_mutex.Lock();
    if(buf.getSize() < buf.maxSize()){
        buf.push(packet);
        m_mutex.Unlock();
        std::cout<<"add packet == packetNum"<<buf.getSize()<<"--"<<Simulator::Now()<<std::endl;
    }
    else{
        m_mutex.Unlock();
        ++lossPacketNum;
        std::cout<<"loss packet"<<lossPacketNum<<std::endl;
    }

}
//send function
void SendMySock(Ptr<Socket> socket,Ptr<Packet> packet,InetSocketAddress remote){
    socket->SendTo(packet,0,remote);
}
// process packet function in thread ,but do nothing
void processThread(){
}
class threadPool{
    public:
        void dealPacket();
};

Ptr<SystemThread> st[threadNum];    //initial thread array

//thread use it to get packet from buffer
void threadPool::dealPacket(){
    int flag = 0;
//    while(flag < 100000){
      while(1){
        m_mutex.Lock();
        if(buf.getSize() > 1){
            Ptr<Packet> packet = buf.getPacket();
            m_mutex.Unlock();
            
            std::cout<<"dealPacket "<<dealPacketNum++<<" "<<Simulator::Now()<<"--"<<buf.getSize()<<std::endl;
            Simulator::ScheduleWithContext(0xffffffff,Seconds(0.0),MakeEvent(&processThread));
            usleep(usleepTime);//the time processor process the packet
            flag = 0;
        }
        else{
            m_mutex.Unlock();
            usleep(10);
            flag++;
        }
    }
}
//start the thread
void threadToDeal(){
    threadPool thp;
    for(int i=0;i<threadNum;++i){
        st[i] = Create<SystemThread>(MakeCallback(&threadPool::dealPacket,&thp));
        st[i]->Start();
    }
}
int main(){
    NodeContainer node;
    node.Create(5);
    
    NetDeviceContainer dev;
    CsmaHelper csma;

    dev = csma.Install(node);

    InternetStackHelper internet;
    internet.Install(node);

    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.0.0.0","255.255.255.0");
    Ipv4InterfaceContainer ifc = ipv4.Assign(dev);

    std::cout<<"----------"<<std::endl;
    Ptr<Packet> packet = Create<Packet>();
    
    InetSocketAddress remote = InetSocketAddress(ifc.GetAddress(4),8086);

    //receive socket
    TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
    Ptr<Socket> recvSock = Socket::CreateSocket(node.Get(4),tid);
    recvSock->Bind(remote);
    recvSock->SetRecvCallback(MakeCallback(&RecvFunc));


    //send socket
    Ptr<Socket> sendSock[4];
    InetSocketAddress local[4] = InetSocketAddress(8086);
    for(int j = 0; j < 4 ; ++j){
        sendSock[j] = Socket::CreateSocket(node.Get(j),tid);
        local[j].SetIpv4(ifc.GetAddress(j));
    }
    
    for(int k = 0; k < 4 ; ++k){
            sendSock[k]->Bind(local[k]);
    }

    //start thread array
    threadToDeal();

    //start to send packet
    while(sendPacketNum < defineTotalPacketNum ){
        for(int i=0 ; i < 4 ; ++i){
//            std::cout<<(float)sendPacketNum/10000+0.01<<std::endl;
            Simulator::Schedule(Seconds(0.01+(float)sendPacketNum/100000),&SendMySock,sendSock[i],packet,remote);
  //          std::cout<<ifc.GetAddress(i)<<" "<<sendPacketNum<<" "<<ifc.GetAddress(4)<<std::endl;
            sendPacketNum++;
        }
    }


    Simulator::Stop(Seconds(10000000.0));
    Simulator::Run();
    //stop thread
    int j;
    for(j=0;j<threadNum;++j){
      st[j]->Join();
    }
    
    Simulator::Destroy();
    return 0;
    
}

