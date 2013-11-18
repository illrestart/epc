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
        std::cout<<"--------Session Send Count---------:"<<sessionCount<<std::endl;
}

SessionApplication::SessionApplication(Ptr<Node> node,InetSocketAddress local,InetSocketAddress remote,int totalNumber, float rate,float time)
	:m_node(node),
	m_remote(remote),
	m_local(local),
	m_socket(0),
	m_lastStartTime(Seconds(0.0)),
	m_totalNumber(totalNumber),
	m_rate(rate),
	m_time(time),
	sessionCount(0)
	
{
	tmpTime = 0.0;
	if( (time == 0.0) || (totalNumber == 0) || (rate == 0.0) ){
		m_flag = false;
	}
	else{
		m_interval =1.0 / (totalNumber * rate);
		m_flag = true;
	}
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
	ScheduleStartEvent();
}
void SessionApplication::StopApplication(){
	NS_LOG_FUNCTION(this);

}
void SessionApplication::CancelEvents(){

}
void SessionApplication::ScheduleStartEvent(){

	StartSending();
}
void SessionApplication::StartSending(){
	while( m_flag && (tmpTime < m_time) ){
		tmpTime += m_interval;
		std::cout<<m_lastStartTime<<std::endl;
		m_lastStartTime = Seconds(tmpTime);
		Simulator::Schedule(m_lastStartTime,&SessionApplication::sendPacket,this);

	}
	std::cout<<"session++++++++++++++++++++"<<m_lastStartTime<<std::endl;

}
void SessionApplication::sendPacket(){
	static int count;
	Ptr<Packet> packet = Create<Packet>();
	lteEpcTag tag;
	tag.setM_Session();
	tag.setM_SessionServiceRequest();
	tag.m_count = count;
	packet->AddPacketTag(tag);
	m_socket->SendTo(packet,0,m_remote);
	std::cout<<count++<<" send packet:"<<Simulator::Now()<<std::endl;
	sessionCount++;
}
}
