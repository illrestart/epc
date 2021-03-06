
#include "controller-application.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE("controllerApplication");

namespace ns3{

TypeId controllerApplication::GetTypeId(void){
	static TypeId tid = TypeId("ns3::controllerApplication")
		.SetParent<Object>();
	return tid;
}

void controllerApplication::DoDispose(void){
	NS_LOG_FUNCTION(this);
}

controllerApplication::controllerApplication(Ptr<Node> controllerNode,Ipv4InterfaceContainer mmeControllerIfc,int mmeport,Ipv4InterfaceContainer controllerUgwIfc,int ugwport)
	:m_controllerNode(controllerNode),
	m_mmeControllerIfc(mmeControllerIfc),
        m_controllerUgwIfc(controllerUgwIfc),
	m_mmePort(mmeport),
	m_ugwPort(ugwport)
{
	m_port = 8086;

	InitSocket();
	

}
controllerApplication::~controllerApplication(void){
	NS_LOG_FUNCTION(this);
}
void controllerApplication::InitRecvSocket(){
	m_socketMme = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketMme->Bind(InetSocketAddress(m_mmeControllerIfc.GetAddress(1),m_port));

	m_socketUgw = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketUgw->Bind(InetSocketAddress(m_controllerUgwIfc.GetAddress(0),m_port));
}
void controllerApplication::InitSendSocket(){
	m_sendToMmeSocket = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToMmeSocket->Bind(InetSocketAddress(m_mmeControllerIfc.GetAddress(1),8084));

	m_sendToUgwSocket = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToUgwSocket->Bind(InetSocketAddress(m_controllerUgwIfc.GetAddress(0),8085));
	
}
void controllerApplication::InitSocket(){
	InitRecvSocket();

	InitSendSocket();
}
void controllerApplication::StartApplication(){
	NS_LOG_FUNCTION(this);	
	m_socketMme->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));
	m_socketUgw->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));
}
void controllerApplication::StopApplication(){
	NS_LOG_FUNCTION(this);	
}
void controllerApplication::ProcessSession(lteEpcTag tag){
	Ptr<Packet> packetSend = Create<Packet>();
	lteEpcTag tagSend;
	tagSend.m_count = tag.m_count;
	std::cout<<"controller\t: ";
	if(tag.m_status == (uint8_t)m_SessionUplinkData){
		std::cout<<"receive uplinkdata ";
	}
	else if(tag.m_status == (uint8_t)m_SessionModifyBearerRequest){
		std::cout<<"rceive mme  modifybearerrequest ";
		tagSend.setM_Session();
		tagSend.setM_SessionModifyBearerRequest();
		packetSend->AddPacketTag(tagSend);
		m_sendToUgwSocket->SendTo(packetSend,0,InetSocketAddress(m_controllerUgwIfc.GetAddress(1),8086));
	}
	else if(tag.m_status == (uint8_t)m_SessionModifyBearerResponse){
		std::cout<<"rceive ugw modifycearerreqsponse ";
		tagSend.setM_Session();
		tagSend.setM_SessionModifyBearerResponse();
		packetSend->AddPacketTag(tagSend);
		m_sendToMmeSocket->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc.GetAddress(0),m_mmePort));
	}
	std::cout<<"\t:tag number"<<tag.m_count<<std::endl;
} 
void controllerApplication::ProcessPacket(Ptr<Packet> packet){
	lteEpcTag tag;
	packet->RemovePacketTag(tag);
	if(tag.m_flag == (uint8_t)m_Session){
		ProcessSession(tag);	
	}
}
void controllerApplication::RecvFromMmeSocket(Ptr<Socket> socket){
	Ptr<Packet> packet = socket->Recv();
	Simulator::Schedule(Simulator::Now(),&controllerApplication::ProcessPacket,this,packet);

}
	

}
