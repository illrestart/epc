
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

mmeApplication::mmeApplication(Ptr<Node> mmeNode,NodeContainer mmec,NodeContainer enbc,NodeContainer controllerc,Ipv4InterfaceContainer ifc)
	:m_mmeNode(mmeNode),
	m_mmec(mmec),
        m_enbc(enbc),
	m_controllerc(controllerc),
	m_ifc(ifc)
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
//	InitRecvSocket();

//	InitSendSocket();
	m_socket = Socket::CreateSocket(m_mmeNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socket->Bind(InetSocketAddress(m_ifc.GetAddress(m_mmeNode->GetId()),8086));

}

void mmeApplication::StartApplication(){
	NS_LOG_FUNCTION(this);	
//	m_socketEnb->SetRecvCallback(MakeCallback(&mmeApplication::RecvFromEnbSocket,this));
//	m_socketController->SetRecvCallback(MakeCallback(&mmeApplication::RecvFromEnbSocket,this));
	m_socket->SetRecvCallback(MakeCallback(&mmeApplication::RecvFromEnbSocket,this));
}
void mmeApplication::StopApplication(){
	NS_LOG_FUNCTION(this);	
}
//use to set the tag in packet and then send them to designaed address
void mmeApplication::SetStatus(lteEpcTag tag,uint8_t flag,uint8_t status,InetSocketAddress isa){
	Ptr<Packet> packet = Create<Packet>();
	tag.m_flag = flag;
	tag.m_status = status;
	packet->AddPacketTag(tag);
	m_socket->SendTo(packet,0,isa);
}
void mmeApplication::ProcessSession(lteEpcTag tag){
	std::cout<<"mme\t\t: ";
	if(tag.m_status == (uint8_t)m_SessionServiceRequest){
		std::cout<<"receive ue service Request ";
		SetStatus(tag,(uint8_t)m_Session,(uint8_t)m_SessionInitialContextSetupRequest,InetSocketAddress(m_ifc.GetAddress(m_enbc.Get(0)->GetId()),8086));
	}
	else if(tag.m_status == (uint8_t)m_SessionUplinkData){
		std::cout<<"receive enb uplinkdata ";
		SetStatus(tag,(uint8_t)m_Session,(uint8_t)m_SessionUplinkData,InetSocketAddress(m_ifc.GetAddress(m_controllerc.Get(0)->GetId()),8086));
	}
	else if(tag.m_status == (uint8_t)m_SessionInitialContextSetupComplete){
		std::cout<<"rceive enb contextsetupcomplete ";
		SetStatus(tag,(uint8_t)m_Session,(uint8_t)m_SessionModifyBearerRequest,InetSocketAddress(m_ifc.GetAddress(m_controllerc.Get(0)->GetId()),8086));
	}
	else if(tag.m_status == (uint8_t)m_SessionModifyBearerResponse){
		std::cout<<"receive controller modify bearer response ";
	}
	std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;
} 

void  mmeApplication::ProcessHandover(lteEpcTag tag){
	std::cout<<"mme\t\t: ";
	if(tag.m_status == (uint8_t)m_HandoverPathSwitchRequest){
		std::cout<<"receive Path Switch Request from TargetEnb";
		SetStatus(tag,(uint8_t)m_Handover,(uint8_t)m_HandoverModifyBearerRequest,InetSocketAddress(m_ifc.GetAddress(m_controllerc.Get(0)->GetId()),8086));
	}
        else if(tag.m_status == (uint8_t)m_HandoverModifyBearerResponse){
               std::cout<<"receive Modify Bearer Response from controller";
        }
        else if(tag.m_status == (uint8_t)m_HandoverEndMarkerToTargetEnb){
		std::cout<<"receive End Marker To TargetEnb from controller";
		SetStatus(tag,(uint8_t)m_Handover,(uint8_t)m_HandoverEndMarkerToTargetEnb,InetSocketAddress(m_ifc.GetAddress(m_enbc.Get(0)->GetId()),8086));
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
