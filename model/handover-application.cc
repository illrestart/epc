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

HandoverApplication::HandoverApplication(NodeContainer uec,NodeContainer enbc,Ipv4InterfaceContainer ifc,int totalNumber, float rate,float time)
	:m_uec(uec),
	m_enbc(enbc),
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
		m_socket->Bind(InetSocketAddress(m_ifc.GetAddress(m_uec.Get(0)->GetId()),8086));
	}

	ScheduleStartEvent();
}
void HandoverApplication::StopApplication(){,e
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
	m_socket->SendTo(packet,0,InetSocketAddress(m_ifc.GetAddress(m_enb.Get(0)->GetId()),8086));
	std::cout<<count++<<" send packet:"<<Simulator::Now()<<std::endl;
	handoverCount++;
}

}
