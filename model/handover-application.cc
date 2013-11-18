#include "handover-application.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE("HandoverApplication");

namespace ns3{



TypeId HandoverApplication::GetTypeId(void){
	static TypeId tid = TypeId("ns3::HandoverApplication")
		.SetParent<Object>();
	return tid;
}

void HandoverApplication::DoDispose(void){
	NS_LOG_FUNCTION(this);
        std::cout<<"--------Handover Send Count---------:"<<handoverCount<<std::endl;
}

HandoverApplication::HandoverApplication(Ptr<Node> node,InetSocketAddress local,InetSocketAddress remote,int totalNumber, float rate,float time)
	:m_node(node),
	m_remote(remote),
	m_local(local),
	m_socket(0),
	m_lastStartTime(Seconds(0.0)),
	m_totalNumber(totalNumber),
	m_rate(rate),
	m_time(time),
	handoverCount(0)
	
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
HandoverApplication::~HandoverApplication(void){
	NS_LOG_FUNCTION(this);
}

void HandoverApplication::StartApplication(){
	NS_LOG_FUNCTION(this);
	if(! m_socket){
		m_socket = Socket::CreateSocket(m_node,TypeId::LookupByName("ns3::UdpSocketFactory"));
		m_socket->Bind(m_local);
	}

	ScheduleStartEvent();
}
void HandoverApplication::StopApplication(){
	NS_LOG_FUNCTION(this);

}
void HandoverApplication::CancelEvents(){

}
void HandoverApplication::ScheduleStartEvent(){
	StartSending();
}
void HandoverApplication::StartSending(){
	while( m_flag && (tmpTime < m_time) ){
		tmpTime += m_interval;
		std::cout<<m_lastStartTime<<std::endl;
		m_lastStartTime = Seconds(tmpTime);
		Simulator::Schedule(m_lastStartTime,&HandoverApplication::sendPacket,this);

	}

	std::cout<<"handover++++++++++++++++++++"<<m_lastStartTime<<std::endl;

}
void HandoverApplication::sendPacket(){
	static int count;
	Ptr<Packet> packet = Create<Packet>();
	lteEpcTag tag;
	tag.setM_Handover();
	tag.setM_HandoverPathSwitchRequest();
	tag.m_count = count;
	packet->AddPacketTag(tag);
	m_socket->SendTo(packet,0,m_remote);
	std::cout<<count++<<" send packet:"<<Simulator::Now()<<std::endl;
	handoverCount++;
}

}
