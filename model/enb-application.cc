#include <unistd.h>
#include "enb-application.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE("enbApplication");

namespace ns3{

TypeId enbApplication::GetTypeId(void){
	static TypeId tid = TypeId("ns3::enbApplication")
		.SetParent<Object>();
	return tid;
}

void enbApplication::DoDispose(void){
	NS_LOG_FUNCTION(this);
}

enbApplication::enbApplication(Ptr<Node> enbNode,Ipv4InterfaceContainer ueEnbIfc,int ueport,Ipv4InterfaceContainer enbMmeIfc,int mmeport,Ipv4InterfaceContainer ugwEnbIfc,int ugwport)
	:m_enbNode(enbNode),
	m_ueEnbIfc(ueEnbIfc),
        m_enbMmeIfc(enbMmeIfc),
        m_ugwEnbIfc(ugwEnbIfc),
	m_uePort(ueport),
	m_mmePort(mmeport),
	m_ugwPort(ugwport)
{
	m_port = 8086;
	InitSocket();
}
enbApplication::~enbApplication(void){
	NS_LOG_FUNCTION(this);
}
void enbApplication::InitRecvSocket(){
	m_socketUe = Socket::CreateSocket(m_enbNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketUe->Bind(InetSocketAddress(m_ueEnbIfc.GetAddress(1),8086));

	m_socketMme = Socket::CreateSocket(m_enbNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketMme->Bind(InetSocketAddress(m_enbMmeIfc.GetAddress(0),8086));
	
	m_socketUgw = Socket::CreateSocket(m_enbNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketUgw->Bind(InetSocketAddress(m_ugwEnbIfc.GetAddress(1),8086));
}
void enbApplication::InitSendSocket(){
	m_sendToUeSocket = Socket::CreateSocket(m_enbNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToUeSocket->Bind(InetSocketAddress(m_ueEnbIfc.GetAddress(1),8085));

	m_sendToMmeSocket = Socket::CreateSocket(m_enbNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToMmeSocket->Bind(InetSocketAddress(m_enbMmeIfc.GetAddress(0),8085));


	m_sendToUgwSocket = Socket::CreateSocket(m_enbNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToUgwSocket->Bind(InetSocketAddress(m_ugwEnbIfc.GetAddress(1),8085));

}
void enbApplication::InitSocket(){
	InitRecvSocket();

	InitSendSocket();
}
void enbApplication::StartApplication(){
	NS_LOG_FUNCTION(this);	
	m_socketUe->SetRecvCallback(MakeCallback(&enbApplication::RecvFromUeSocket,this));
	m_socketMme->SetRecvCallback(MakeCallback(&enbApplication::RecvFromUeSocket,this));
	m_socketUgw->SetRecvCallback(MakeCallback(&enbApplication::RecvFromUeSocket,this));
}
void enbApplication::StopApplication(){
	NS_LOG_FUNCTION(this);	
}
void enbApplication::ProcessSession(lteEpcTag tag){
	Ptr<Packet> packetSend = Create<Packet>();
	lteEpcTag tagSend;
	tagSend.m_count = tag.m_count;
	std::cout<<"enb\t\t: ";
	if(tag.m_status == (uint8_t)m_SessionServiceRequest){
		std::cout<<"receive ue request ";
		tagSend.setM_Session();
		tagSend.setM_SessionServiceRequest();
		packetSend->AddPacketTag(tagSend);
		m_sendToMmeSocket->SendTo(packetSend,0,InetSocketAddress(m_enbMmeIfc.GetAddress(1),m_mmePort));	
	}
	else if(tag.m_status == (uint8_t)m_SessionInitialContextSetupRequest){
		std::cout<<"receive mme initialcontextsetuprequest ";
		tagSend.setM_Session();
		tagSend.setM_SessionRadioBearerEstablishment();
		packetSend->AddPacketTag(tagSend);
		m_sendToUeSocket->SendTo(packetSend,0,InetSocketAddress(m_ueEnbIfc.GetAddress(0),m_uePort));
	}
	else if(tag.m_status == (uint8_t)m_SessionUplinkData){
		std::cout<<"receive ue uplinkdata ";
		tagSend.setM_Session();
		tagSend.setM_SessionInitialContextSetupComplete();
		packetSend->AddPacketTag(tagSend);
		m_sendToMmeSocket->SendTo(packetSend,0,InetSocketAddress(m_enbMmeIfc.GetAddress(1),m_mmePort));
	}
	std::cout<<"\t:tag number"<<tag.m_count<<std::endl;
} 
void enbApplication::ProcessPacket(Ptr<Packet> packet){
	lteEpcTag tag;
	packet->RemovePacketTag(tag);

	if(tag.m_flag == (uint8_t)m_Session){
		ProcessSession(tag);	
	}
}

void enbApplication::RecvFromUeSocket(Ptr<Socket> socket){	
	Ptr<Packet> packet = socket->Recv();
	Simulator::Schedule(Simulator::Now(),&enbApplication::ProcessPacket,this,packet);

}


}
