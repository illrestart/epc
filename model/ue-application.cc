
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

ueApplication::ueApplication(Ptr<Node> ueNode,NodeContainer enbc,Ipv4InterfaceContainer ifc)
	:m_ueNode(ueNode),
	m_enbc(enbc),
	m_ifc(ifc)
{
	m_port = 8086;
	InitSocket();
	init
}

ueApplication::~ueApplication(void){
	NS_LOG_FUNCTION(this);
}
void ueApplication::InitRecvSocket(){
	TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
	m_socketEnb = Socket::CreateSocket(m_ueNode,tid);
	m_socketEnb->Bind(InetSocketAddress(m_ifc.GetAddress(ueNode->GetId()),m_port));
}
void ueApplication::InitSendSocket(){
	TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
	m_sendToEnbSocket = Socket::CreateSocket(m_ueNode,tid);
	m_sendToEnbSocket->Bind(InetSocketAddress(m_ifc.GetAddress(ueNode->GetId()),8085));
}
void ueApplication::InitSocket(){
/*	InitRecvSocket();
	InitSendSocket();
*/
	TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
	m_socket = Socket::CreateSocket(m_ueNode,tid);
	m_socket->Bind(InetSocketAddress(m_ifc.GetAddress(m_ueNode->GetId()),m_port));
}

void ueApplication::StartApplication(){
	NS_LOG_FUNCTION(this);	
//	m_socketEnb->SetRecvCallback(MakeCallback(&ueApplication::RecvFromEnbSocket,this));
	m_socket->SetRecvCallback(MakeCallback(&ueApplication::RecvFromEnbSocket,this));
}
void ueApplication::StopApplication(){
	NS_LOG_FUNCTION(this);	
}

//use to set the tag in packet and then send them to designaed address
void ueApplication::SetStatus(lteEpcTag tag,uint8_t flag,uint8_t status,InetSocketAddress isa){
	Ptr<Packet> packet = Create<Packet>();
	tag.m_flag = flag;
	tag.m_status = status;
	packet->AddPacketTag(tag);
	m_socket->SendTo(packet,0,isa);
}
void ueApplication::ProcessSession(lteEpcTag tag){
	std::cout<<"ue\t\t: ";
	if(tag.m_status == (uint8_t)m_SessionRadioBearerEstablishment){
		std::cout<<"receive enb radiobeareestablishment ";
		SetStatus(tag,(uint8_t)m_Session,(uint8_t)m_SessionUplinkData,InetSocketAddress(m_ifc.GetAddress(enbc.Get(0)->GetId()),8086))
	}
//	std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;
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
//	Simulator::Schedule(Simulator::Now(),&ueApplication::ProcessPacket,this,packet);
	Simulator::Schedule(Seconds(0.0),&ueApplication::ProcessPacket,this,packet);
}

}
