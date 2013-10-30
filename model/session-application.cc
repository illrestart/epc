#include "session-application.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE("SessionApplication");

namespace ns3{

TypeId SessionApplication::GetTypeId(void){
	static TypeId tid = TypeId("ns3::SessionApplication")
		.SetParent<Object>();
	return tid;
}

void SessionApplication::DoDispose(void){
	NS_LOG_FUNCTION(this);
}

SessionApplication::SessionApplication(Ptr<Node> node,InetSocketAddress local,InetSocketAddress remote)
	:m_node(node),
	m_remote(remote),
	m_local(local),
	m_socket(0),
	m_lastStartTime(Seconds(0.0))
{
	tmpTime = 0.0;
}
SessionApplication::~SessionApplication(void){
	NS_LOG_FUNCTION(this);
}

void SessionApplication::StartApplication(){
	NS_LOG_FUNCTION(this);
	if(! m_socket){
		m_socket = Socket::CreateSocket(m_node,TypeId::LookupByName("ns3::UdpSocketFactory"));
		m_socket->Bind(m_local);
	}
//	CancelEvents();
	ScheduleStartEvent();
}
void SessionApplication::StopApplication(){
	NS_LOG_FUNCTION(this);
//	CancelEvents();
}
void SessionApplication::CancelEvents(){
//	Simulator::Cancel(m_SendEvent);
//	Simulator::Cancel(m_startSendEvent);
}
void SessionApplication::ScheduleStartEvent(){
//	m_startSendEvent = Simulator::Schedule(Seconds(0.0),&SessionApplication::StartSending,this);
	StartSending();
}
void SessionApplication::StartSending(){
	while(tmpTime < 1){
		tmpTime += 0.01;
		std::cout<<m_lastStartTime<<std::endl;
		m_lastStartTime = Seconds(tmpTime);
		Simulator::Schedule(m_lastStartTime,&SessionApplication::sendPacket,this);
//		sendPacket();
	}
//	while(Simulator::Now() < Seconds(40.0)){}

	std::cout<<"++++++++++++++++++++"<<m_lastStartTime<<std::endl;
//	StopApplication();	
}
void SessionApplication::sendPacket(){
	static int count;
	Ptr<Packet> packet = Create<Packet>();
	lteEpcTag tag;
	tag.m_count = count;
	tag.setM_Session();
	tag.setM_SessionServiceRequest();
	packet->AddPacketTag(tag);
	m_socket->SendTo(packet,0,m_remote);
	std::cout<<count++<<" send packet:"<<Simulator::Now()<<std::endl;

}
}
