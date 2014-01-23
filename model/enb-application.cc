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

enbApplication::enbApplication(Ptr<Node> enbNode,NodeContainer enbc,NodeContainer uec,NodeContainer mmec,NodeContainer ugwc,Ipv4InterfaceContainer ifc)
	:m_enbNode(enbNode),
	m_enbc(enbc),
	m_uec(uec),
	m_mmec(mmec),
	m_ugwc(ugwc),
	m_ifc(ifc)
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
//	InitRecvSocket();

//	InitSendSocket();
	m_socket = Socket::CreateSocket(m_enbNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socket->Bind(InetSocketAddress(m_ifc.GetAddress(m_enbNode->GetId()),8086));
}
void enbApplication::StartApplication(){
	NS_LOG_FUNCTION(this);	
/*	m_socketUe->SetRecvCallback(MakeCallback(&enbApplication::RecvFromUeSocket,this));
	m_socketMme->SetRecvCallback(MakeCallback(&enbApplication::RecvFromUeSocket,this));
	m_socketUgw->SetRecvCallback(MakeCallback(&enbApplication::RecvFromUeSocket,this));
*/
	m_socket->SetRecvCallback(MakeCallback(&enbApplication::RecvFromUeSocket,this));
/*test*/
/*	Ptr<Packet> packetSend = Create<Packet>();
	lteEpcTag tagSend;

	tagSend.setM_Handover();
	tagSend.setM_HandoverPathSwitchRequest();
	packetSend->AddPacketTag(tagSend);
	
	m_sendToMmeSocket->SendTo(packetSend,0,InetSocketAddress(m_enbMmeIfc.GetAddress(1),m_mmePort));	
*/
}
void enbApplication::StopApplication(){
	NS_LOG_FUNCTION(this);	
}
//use to set the tag in packet and then send them to designaed address
void enbApplication::SetStatus(lteEpcTag tag,uint8_t flag,uint8_t status,InetSocketAddress isa){
	Ptr<Packet> packet = Create<Packet>();
	tag.m_flag = flag;
	tag.m_status = status;
	packet->AddPacketTag(tag);
	m_socket->SendTo(packet,0,isa);
}
void enbApplication::ProcessSession(lteEpcTag tag){
	std::cout<<"enb\t\t: ";
	if(tag.m_status == (uint8_t)m_SessionServiceRequest){
		std::cout<<"receive ue request ";
		SetStatus(tag,(uint8_t)m_Session,(uint8_t)m_SessionServiceRequest,InetSocketAddress(m_ifc.GetAddress(m_mmec.Get(0)->GetId()),8086));
	}
	else if(tag.m_status == (uint8_t)m_SessionInitialContextSetupRequest){
		std::cout<<"receive mme initialcontextsetuprequest ";
		SetStatus(tag,(uint8_t)m_Session,(uint8_t)m_SessionRadioBearerEstablishment,InetSocketAddress(m_ifc.GetAddress(m_uec.Get(0)->GetId()),8086));
	}
	else if(tag.m_status == (uint8_t)m_SessionUplinkData){
		std::cout<<"receive ue uplinkdata ";
		SetStatus(tag,(uint8_t)m_Session,(uint8_t)m_SessionInitialContextSetupComplete,InetSocketAddress(m_ifc.GetAddress(m_mmec.Get(0)->GetId()),8086));
	}
	std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;
} 

void enbApplication::ProcessHandover(lteEpcTag tag){
	Ptr<Packet> packetSend = Create<Packet>();
	lteEpcTag tagSend;
	tagSend.m_count = tag.m_count;
	std::cout<<"enb\t\t: handover";
	if(tag.m_status == (uint8_t)m_HandoverEndMarkerToTargetEnb){
		std::cout<<"TargetENB receives End Marker from mme";
	}
        else if(tag.m_status == (uint8_t)m_HandoverPathSwitchRequestAck){
                std::cout<<"TargetENB receives Path Switch Request Ack from mme";
        }
        std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;
}

void enbApplication::ProcessPacket(Ptr<Packet> packet){
	lteEpcTag tag;
	packet->RemovePacketTag(tag);

	if(tag.m_flag == (uint8_t)m_Session){
		ProcessSession(tag);	
	}
        else if(tag.m_flag == (uint8_t)m_Handover){
                ProcessHandover(tag);
        }
}

void enbApplication::RecvFromUeSocket(Ptr<Socket> socket){	
	Ptr<Packet> packet = socket->Recv();
//	Simulator::Schedule(Simulator::Now(),&enbApplication::ProcessPacket,this,packet);
        Simulator::Schedule(Seconds(0.0),&enbApplication::ProcessPacket,this,packet);

}

}
