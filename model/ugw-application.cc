
#include "ugw-application.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE("ugwApplication");

namespace ns3{

TypeId ugwApplication::GetTypeId(void){
	static TypeId tid = TypeId("ns3::ugwApplication")
		.SetParent<Object>();
	return tid;
}

void ugwApplication::DoDispose(void){
	NS_LOG_FUNCTION(this);
}

ugwApplication::ugwApplication(Ptr<Node> ugwNode,NodeContainer enb,NodeContainer controller,Ipv4InterfaceContainer ifc)
	:m_ugwNode(ugwNode),
	m_enbc(enb),
        m_controllerc(controller),
	m_ifc(ifc)
{
	m_port = 8086;
	InitSocket();
}
ugwApplication::~ugwApplication(void){
	NS_LOG_FUNCTION(this);
}
void ugwApplication::InitRecvSocket(){
	m_socketController = Socket::CreateSocket(m_ugwNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketController->Bind(InetSocketAddress(m_controllerUgwIfc.GetAddress(1),8086));

	m_socketEnb = Socket::CreateSocket(m_ugwNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketEnb->Bind(InetSocketAddress(m_ugwEnbIfc.GetAddress(0),m_port));

}
void ugwApplication::InitSendSocket(){
	m_sendToControllerSocket = Socket::CreateSocket(m_ugwNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToControllerSocket->Bind(InetSocketAddress(m_controllerUgwIfc.GetAddress(1),8085));

	m_sendToEnbSocket = Socket::CreateSocket(m_ugwNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToEnbSocket->Bind(InetSocketAddress(m_ugwEnbIfc.GetAddress(0),8084));

}
void ugwApplication::InitSocket(){
//	InitRecvSocket();

//	InitSendSocket();
	m_socket  = Socket::CreateSocket(m_ugwNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socket->Bind(InetSocketAddress(m_ifc.GetAddress(m_ugwNode->GetId()),8086));
}

void ugwApplication::StartApplication(){
	NS_LOG_FUNCTION(this);	
//	m_socketController->SetRecvCallback(MakeCallback(&ugwApplication::RecvFromEnbSocket,this));
//	m_socketEnb->SetRecvCallback(MakeCallback(&ugwApplication::RecvFromEnbSocket,this));
	m_socket->SetRecvCallback(MakeCallback(&ugwApplication::RecvFromEnbSocket,this));

}
void ugwApplication::StopApplication(){
	NS_LOG_FUNCTION(this);	
}
//use to set the tag in packet and then send them to designaed address
void ugwApplication::SetStatus(lteEpcTag tag,uint8_t flag,uint8_t status,InetSocketAddress isa){
	Ptr<Packet> packet = Create<Packet>();
	tag.m_flag = flag;
	tag.m_status = status;
	packet->AddPacketTag(tag);
	m_socket->SendTo(packet,0,isa);
}
void ugwApplication::ProcessSession(lteEpcTag tag){
	std::cout<<"ugw\t\t: ";
	if(tag.m_status == (uint8_t)m_SessionModifyBearerRequest){
		std::cout<<"receive controller modifybearerrequest ";
		SetStatus(tag,(uint8_t)m_Session,(uint8_t)m_SessionModifyBearerResponse,InetSocketAddress(m_ifc.GetAddress(m_controllerc.Get(0)->GetId()),8086));
	}
	std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;
} 

void ugwApplication::ProcessHandover(lteEpcTag tag){
	std::cout<<"ugw\t\t: ";
	if(tag.m_status == (uint8_t)m_HandoverModifyBearerRequestToUgw){
		std::cout<<"receive Modify Bearer Request from controller";
		SetStatus(tag,(uint8_t)m_Handover,(uint8_t)m_HandoverModifyBearerResponseToUgw,InetSocketAddress(m_ifc.GetAddress(m_controllerc.Get(0)->GetId()),8086));
	}
	std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;
} 

void ugwApplication::ProcessPacket(Ptr<Packet> packet){
	lteEpcTag tag;
	packet->RemovePacketTag(tag);
	if(tag.m_flag == (uint8_t)m_Session){
		ProcessSession(tag);	
	}
	else if(tag.m_flag == (uint8_t)m_Handover){
		ProcessHandover(tag);	
	}

}
void ugwApplication::RecvFromEnbSocket(Ptr<Socket> socket){
	Ptr<Packet> packet = socket->Recv();
//	Simulator::Schedule(Simulator::Now(),&ugwApplication::ProcessPacket,this,packet);
	Simulator::Schedule(Seconds(0.0),&ugwApplication::ProcessPacket,this,packet);
}

}
