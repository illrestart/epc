#include "ns3/node_buffer.h"

//NS_LOG_COMPONENT_DEFINE("lteEpcBuffer");

namespace ns3{

/*TypeId lteEpcBuffer::GetTypeId(void){
    static TypeId tid = TypeId("ns3::lteEpcBuffer")
        .AddConstructor<lteEpcBuffer>()
        ;
    return tid;
}
*/
int lteEpcBuffer::getSize(){
    return m_queue.size();
}

int lteEpcBuffer::maxSize(){
    return maxsize;
}

lteEpcPacket lteEpcBuffer::getPacket(){
    lteEpcPacket packet = m_queue.front();
    m_queue.pop();
    return packet;
}

void lteEpcBuffer::push(lteEpcPacket packet){
    m_queue.push(packet);
}
}
