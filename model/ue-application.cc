
#include "ue-application.h"
#include "ns3/log.h"
#include "ns3/simulator.h"


NS_LOG_COMPONENT_DEFINE("ueApplication");

namespace ns3{

TypeId ueApplication::GetTypeId(void){
	static TypeId tid = TypeId("ns3::ueApplication")
		.SetParent<Object>();
	return tid;
}

void ueApplication::DoDispose(void){
	NS_LOG_FUNCTION(this);
}

ueApplication::ueApplication(Ptr<Node> ueNode,Ipv4InterfaceContainer ueEnbIfc,int enbport)
	:m_ueNode(ueNode),
	m_ueEnbIfc(ueEnbIfc),
	m_enbPort(enbport)
{
	m_port = 8086;
	m_signal.wait();
	InitSocket();
	
}

ueApplication::~ueApplication(void){
	NS_LOG_FUNCTION(this);
}
void ueApplication::InitRecvSocket(){
	m_socketEnb = Socket::CreateSocket(m_ueNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketEnb->Bind(InetSocketAddress(m_ueEnbIfc.GetAddress(0),m_port));
}
void ueApplication::InitSendSocket(){
	m_sendToEnbSocket = Socket::CreateSocket(m_ueNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToEnbSocket->Bind(InetSocketAddress(m_ueEnbIfc.GetAddress(0),8085));
}
void ueApplication::InitSocket(){
	InitRecvSocket();

	InitSendSocket();
}

void ueApplication::StartApplication(){
	NS_LOG_FUNCTION(this);	
	m_socketEnb->SetRecvCallback(MakeCallback(&ueApplication::RecvFromEnbSocket,this));
}
void ueApplication::StopApplication(){
	NS_LOG_FUNCTION(this);	
}

void ueApplication::ProcessSession(lteEpcTag tag){
	Ptr<Packet> packetSend = Create<Packet>();
	lteEpcTag tagSend;
	tagSend.m_count = tag.m_count;
	std::cout<<"ue\t\t: ";
	if(tag.m_status == (uint8_t)m_SessionRadioBearerEstablishment){
		std::cout<<"receive enb radiobeareestablishment ";
		tagSend.setM_Session();
		tagSend.setM_SessionUplinkData();
		packetSend->AddPacketTag(tagSend);
		m_sendToEnbSocket->SendTo(packetSend,0,InetSocketAddress(m_ueEnbIfc.GetAddress(1),m_enbPort));
	}
	std::cout<<"\t:tag number"<<tag.m_count<<std::endl;
} 
void ueApplication::ProcessPacket(Ptr<Packet> packet){
	lteEpcTag tag;
	packet->RemovePacketTag(tag);
	if(tag.m_flag == (uint8_t)m_Session){
		ProcessSession(tag);	
	}
	
}

void ueApplication::RecvFromEnbSocket(Ptr<Socket> socket){
	Ptr<Packet> packet = socket->Recv();
	Simulator::Schedule(Simulator::Now(),&ueApplication::ProcessPacket,this,packet);
}

}
