
#include "mme-application.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE("mmeApplication");

namespace ns3{


TypeId mmeApplication::GetTypeId(void){
	static TypeId tid = TypeId("ns3::mmeApplication")
		.SetParent<Object>();
	return tid;
}

void mmeApplication::DoDispose(void){
	NS_LOG_FUNCTION(this);
}

mmeApplication::mmeApplication(Ptr<Node> mmeNode,Ipv4InterfaceContainer enbMmeIfc,int enbport,Ipv4InterfaceContainer mmeControllerIfc,int controllerport)
	:m_mmeNode(mmeNode),
	m_enbMmeIfc(enbMmeIfc),
        m_mmeControllerIfc(mmeControllerIfc),
	m_enbPort(enbport),
	m_controllerPort(controllerport)
{
	m_port = 8086;
	InitSocket();
}
mmeApplication::~mmeApplication(void){
	NS_LOG_FUNCTION(this);
}
void mmeApplication::InitRecvSocket(){
	m_socketEnb = Socket::CreateSocket(m_mmeNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketEnb->Bind(InetSocketAddress(m_enbMmeIfc.GetAddress(1),8086));

	m_socketController = Socket::CreateSocket(m_mmeNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketController->Bind(InetSocketAddress(m_mmeControllerIfc.GetAddress(0),8086));
}
void mmeApplication::InitSendSocket(){
	m_sendToEnbSocket = Socket::CreateSocket(m_mmeNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToEnbSocket->Bind(InetSocketAddress(m_enbMmeIfc.GetAddress(1),8085));

	m_sendToControllerSocket = Socket::CreateSocket(m_mmeNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToControllerSocket->Bind(InetSocketAddress(m_mmeControllerIfc.GetAddress(0),8084));
}
void mmeApplication::InitSocket(){
	InitRecvSocket();

	InitSendSocket();
}

void mmeApplication::StartApplication(){
	NS_LOG_FUNCTION(this);	
	m_socketEnb->SetRecvCallback(MakeCallback(&mmeApplication::RecvFromEnbSocket,this));
	m_socketController->SetRecvCallback(MakeCallback(&mmeApplication::RecvFromEnbSocket,this));
}
void mmeApplication::StopApplication(){
	NS_LOG_FUNCTION(this);	
}
void mmeApplication::ProcessSession(lteEpcTag tag){
	Ptr<Packet> packetSend = Create<Packet>();
	lteEpcTag tagSend;
	tagSend.m_count = tag.m_count;
	std::cout<<"mme\t\t: ";
	if(tag.m_status == (uint8_t)m_SessionServiceRequest){
		std::cout<<"receive ue service Request ";
		tagSend.setM_Session();
		tagSend.setM_SessionInitialContextSetupRequest();
		packetSend->AddPacketTag(tagSend);
		m_sendToEnbSocket->SendTo(packetSend,0,InetSocketAddress(m_enbMmeIfc.GetAddress(0),m_enbPort));
	}
	else if(tag.m_status == (uint8_t)m_SessionUplinkData){
		std::cout<<"receive enb uplinkdata ";
		tagSend.setM_Session();
		tagSend.setM_SessionUplinkData();
		packetSend->AddPacketTag(tagSend);
		m_sendToControllerSocket->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc.GetAddress(1),m_controllerPort));
	}
	else if(tag.m_status == (uint8_t)m_SessionInitialContextSetupComplete){
		std::cout<<"rceive enb contextsetupcomplete ";
		tagSend.setM_Session();
		tagSend.setM_SessionModifyBearerRequest();
		packetSend->AddPacketTag(tagSend);
		m_sendToControllerSocket->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc.GetAddress(1),m_controllerPort));
	}
	else if(tag.m_status == (uint8_t)m_SessionModifyBearerResponse){
		std::cout<<"receive controller modify bearer response ";
	}
	std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;
} 

void  mmeApplication::ProcessHandover(lteEpcTag tag){
	Ptr<Packet> packetSend = Create<Packet>();
	lteEpcTag tagSend;
	tagSend.m_count = tag.m_count;
	std::cout<<"mme\t\t: ";
	if(tag.m_status == (uint8_t)m_HandoverPathSwitchRequest){
		std::cout<<"receive Path Switch Request from TargetEnb";
		tagSend.setM_Handover();
		tagSend.setM_HandoverModifyBearerRequest();
		packetSend->AddPacketTag(tagSend);
		m_sendToControllerSocket->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc.GetAddress(1),m_controllerPort));
	}
        else if(tag.m_status == (uint8_t)m_HandoverModifyBearerResponse){
               std::cout<<"receive Modify Bearer Response from controller";
        }
        else if(tag.m_status == (uint8_t)m_HandoverEndMarkerToTargetEnb){
		std::cout<<"receive End Marker To TargetEnb from controller";
		tagSend.setM_Handover();
		tagSend.setM_HandoverEndMarkerToTargetEnb();
		packetSend->AddPacketTag(tagSend);
		m_sendToEnbSocket->SendTo(packetSend,0,InetSocketAddress(m_enbMmeIfc.GetAddress(0),m_enbPort));
        }
        std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;
}

void mmeApplication::ProcessPacket(Ptr<Packet> packet){
	lteEpcTag tag;
	packet->RemovePacketTag(tag);
	if(tag.m_flag == (uint8_t)m_Session){
		ProcessSession(tag);	
	}
        else if(tag.m_flag == (uint8_t)m_Handover){
                ProcessHandover(tag);
        }
}

void mmeApplication::RecvFromEnbSocket(Ptr<Socket> socket){
	Ptr<Packet> packet = socket->Recv();
//	Simulator::Schedule(Simulator::Now(),&mmeApplication::ProcessPacket,this,packet);
	Simulator::Schedule(Seconds(0.0),&mmeApplication::ProcessPacket,this,packet);
}

}
