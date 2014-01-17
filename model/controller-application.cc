
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

/*wulei--process result */
	Time end = m_endProcess - m_startRev;
	float interval = end.GetSeconds();
	
	float result = m_sendCount / interval;

	std::cout<<"-------------------receive:-------------------:"<<m_revCount<<std::endl;
	std::cout<<"-------------------processed:-------------------:"<<m_sendCount<<std::endl;

	std::cout<<"-------------------process resutl-------------------:"<<result<<std::endl;
	std::cout<<"------controller receive the first packet at time -----:"<<m_startRev.GetSeconds()<<std::endl;
	std::cout<<"------controller process the last packet at time -----:"<<m_endProcess.GetSeconds()<<std::endl;
	std::cout<<"------lossBUff -----:"<<m_lossBuff<<std::endl;
}

controllerApplication::controllerApplication(Ptr<Node> controllerNode,NodeContainer mmec,NodeContainer ugwc,Ipv4InterfaceContainer ifc)
	:m_controllerNode(controllerNode),
	m_mmec(mmec),
	m_ugwc(ugwc),
	m_ifc(ifc)
{
	m_port = 8086;
	m_sendCount = 0;
	m_revCount = 0;
	m_SocketSourceIpFlag = -1;
	m_maxBuffSize = 1;
	m_buffCount = 0;
	m_lossBuff = 0;

	m_buf.m_maxsize = 10;
	m_threadNum = 1;
	m_processTime = 1000;

/*	for(int i=0;i<4;++i){
		m_mmeControllerIfc[i] = mmeControllerIfc[i];
		m_controllerUgwIfc[i] = controllerUgwIfc[i];
	}*/
	InitSocket();
	
//	cthread = Create<SystemThread>(MakeCallback(&controllerApplication::,this));

}
controllerApplication::~controllerApplication(void){
	NS_LOG_FUNCTION(this);

}
void controllerApplication::InitRecvSocket(){
	TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
	for(int i = 0 ; i < 4 ; ++i){
		m_socketMme[i] = Socket::CreateSocket(m_controllerNode,tid);
		m_socketMme[i]->Bind(InetSocketAddress(m_mmeControllerIfc[i].GetAddress(1),m_port));

		m_socketUgw[i] = Socket::CreateSocket(m_controllerNode,tid);
		m_socketUgw[i]->Bind(InetSocketAddress(m_controllerUgwIfc[i].GetAddress(0),m_port));
	}
}
void controllerApplication::InitSendSocket(){
	TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
	for(int i=0; i<4 ; ++i){
		m_sendToMmeSocket[i] = Socket::CreateSocket(m_controllerNode,tid);
		m_sendToMmeSocket[i]->Bind(InetSocketAddress(m_mmeControllerIfc[i].GetAddress(1),8084));

		m_sendToUgwSocket[i] = Socket::CreateSocket(m_controllerNode,tid);
		m_sendToUgwSocket[i]->Bind(InetSocketAddress(m_controllerUgwIfc[i].GetAddress(0),8085));
	}
}
void controllerApplication::InitSocket(){
//	InitRecvSocket();

//	InitSendSocket();
	TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
	m_socket = Socket::CreateSocket(m_controllerNode,tid);
	m_socket->Bind(InetSocketAddress(m_ifc.GetAddress(m_controllerNode->GetId()),m_port));
}
void controllerApplication::dealPacket(){
    int flag = 0;
    lteEpcPacket packet;
    while(flag < 100000){
        m_mutex.Lock();
        if(m_buf.getSize() > 1){
            packet = m_buf.getPacket();
            m_mutex.Unlock();
            Simulator::ScheduleWithContext(0xffffffff,Seconds(0.0),MakeEvent(&controllerApplication::ProcessPacket,this,packet.m_packet,packet.m_ipadd));
            usleep(m_processTime);
            flag = 0;
        }
        else{
            m_mutex.Unlock();
            usleep(10);
            ++flag;
        }
    }
}
void controllerApplication::StartApplication(){
	NS_LOG_FUNCTION(this);	
	for(int i = 0; i < m_threadNum; ++i){
		m_st[i] = Create<SystemThread>(MakeCallback(&controllerApplication::dealPacket,this));
		m_st[i]->Start();
	}
/*	for(int i=0; i<4 ;++i){
		m_socketMme[i]->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));
		m_socketUgw[i]->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));
	}
*/
	m_socket->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));
}
void controllerApplication::StopApplication(){
	NS_LOG_FUNCTION(this);	
	for(int i = 0; i < m_threadNum; ++i){
		m_st[i]->Join();
	}

}
//ues to define the destination address
InetSocketAddress controllerApplication::getDestinationAddress(Ipv4Address ipadd){
	uint32_t i;
	for(std::vector<lteEpcArea>::iterator iter = m_area.begin(); iter != m_area.end(); ++iter){

		for(i = 0; i < iter->m_mmec.GetN(); ++i){
			if(ipadd == m_ifc.GetAddress(iter->m_mmec.Get(i)->GetId())){
				return InetSocketAddress(m_ifc.GetAddress(iter->m_ugwc.Get(0)->GetId()),8086);
			}
		}
		for(i = 0; i < iter->m_ugwc.GetN(); ++i){
			if(ipadd == m_ifc.GetAddress(iter->m_ugwc.Get(i)->GetId())){
				return InetSocketAddress(m_ifc.GetAddress(iter->m_mmec.Get(0)->GetId()),8086);
			}
		}
	}
	return InetSocketAddress(Ipv4Address("0.0.0.0"),0);
}
//use to set the tag in packet and then send them to designaed address
void controllerApplication::SetStatus(lteEpcTag tag,uint8_t flag,uint8_t status,Ipv4Address ipadd){
	Ptr<Packet> packet = Create<Packet>();
	tag.m_flag = flag;
	tag.m_status = status;
	packet->AddPacketTag(tag);
	InetSocketAddress isa= getDestinationAddress(ipadd);
	std::cout<<isa.GetIpv4()<<std::endl;
	m_socket->SendTo(packet,0,isa);
}
void controllerApplication::ProcessSession(lteEpcTag tag,Ipv4Address ipadd){
	std::cout<<"controller\t: ";
	if(tag.m_status == (uint8_t)m_SessionUplinkData){
		std::cout<<"receive uplinkdata ";
	}
	else{
		//tagSend.setM_Session();
		if(tag.m_status == (uint8_t)m_SessionModifyBearerRequest){
			std::cout<<"rceive mme  modifybearerrequest ";
			SetStatus(tag,(uint8_t)m_Session,(uint8_t)m_SessionModifyBearerRequest,ipadd);
		}
		else if(tag.m_status == (uint8_t)m_SessionModifyBearerResponse){
			std::cout<<"rceive ugw modifycearerreqsponse ";
			SetStatus(tag,(uint8_t)m_Session,(uint8_t)m_SessionModifyBearerResponse,ipadd);
		}

	}
	std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;


	m_endProcess = Simulator::Now();
} 
void controllerApplication::ProcessHandover(lteEpcTag tag,Ipv4Address ipadd){
	std::cout<<"controller\t: ";
	if(tag.m_status == (uint8_t)m_HandoverModifyBearerRequest){
		std::cout<<"receive Modify Bearer Request from mme";
		SetStatus(tag,(uint8_t)m_Handover,(uint8_t)m_HandoverModifyBearerRequestToUgw,ipadd);
//		packetSend->AddPacketTag(tagSend);
//	    controllerSendPacket(packetSend,flag);
	}
	else if(tag.m_status == (uint8_t)m_HandoverModifyBearerResponseToUgw){
		std::cout<<"receive Modify Bearer Response from ugw";
		SetStatus(tag,(uint8_t)m_Handover,(uint8_t)m_HandoverModifyBearerResponse,ipadd);
		SetStatus(tag,(uint8_t)m_Handover,(uint8_t)m_HandoverEndMarkerToTargetEnb,ipadd);

//		packetSend1->AddPacketTag(tagSend1);
//	    controllerSendPacket(packetSend1,flag);
	}


    std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;
	m_endProcess = Simulator::Now();
}
void controllerApplication::ProcessPacket(Ptr<Packet> packet,Ipv4Address ipadd){
	
	lteEpcTag tag;
	packet->RemovePacketTag(tag);
	if(tag.m_flag == (uint8_t)m_Session){
		ProcessSession(tag,ipadd);	
	}
	else if(tag.m_flag == (uint8_t)m_Handover){
        	ProcessHandover(tag,ipadd);
    	}

	/*wulei*/
	m_sendCount++;
}
void controllerApplication::RecvFromMmeSocket(Ptr<Socket> socket){
	Address from;
  	Ptr<Packet> packet = socket->RecvFrom (from);
 	InetSocketAddress m_SocketSourceIp = InetSocketAddress::ConvertFrom (from);
	Ipv4Address ipadd = m_SocketSourceIp.GetIpv4();	
	
	lteEpcPacket pack;
	pack.m_packet = packet;
	pack.m_ipadd = ipadd;
	if(m_buf.getSize() < m_buf.maxSize()){
            m_mutex.Lock();
            m_buf.push(pack);
            m_mutex.Unlock();
        }
        else{
            ++m_lossBuff;
        }
	
//	Simulator::Schedule(Seconds(0.5),&controllerApplication::ProcessPacket,this,packet,ipadd);
}
void controllerApplication::InstallAreaInfo(std::vector<lteEpcArea> area){
	m_area = area;
}

}
