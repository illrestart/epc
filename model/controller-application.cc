
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

controllerApplication::controllerApplication(Ptr<Node> controllerNode,Ipv4InterfaceContainer mmeControllerIfc[],Ipv4InterfaceContainer controllerUgwIfc[],int mmeport,int ugwport)
	:m_controllerNode(controllerNode),
	m_mmePort(mmeport),
	m_ugwPort(ugwport)
{
	m_port = 8086;
	m_sendCount = 0;
	m_revCount = 0;
	m_SocketSourceIpFlag = -1;
	m_maxBuffSize = 1;
	m_buffCount = 0;
	m_lossBuff = 0;

	for(int i=0;i<4;++i){
		m_mmeControllerIfc[i] = mmeControllerIfc[i];
		m_controllerUgwIfc[i] = controllerUgwIfc[i];
	}
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
	InitRecvSocket();

	InitSendSocket();
}
void controllerApplication::StartApplication(){
	NS_LOG_FUNCTION(this);	
	for(int i=0; i<4 ;++i){
		m_socketMme[i]->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));
		m_socketUgw[i]->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));
	}
}
void controllerApplication::StopApplication(){
	NS_LOG_FUNCTION(this);	

}
void controllerApplication::ProcessSession(lteEpcTag tag,int flag){
	Ptr<Packet> packetSend = Create<Packet>();
	lteEpcTag tagSend;
	tagSend.m_count = tag.m_count;
	std::cout<<"controller\t: ";
	if(tag.m_status == (uint8_t)m_SessionUplinkData){
		std::cout<<"receive uplinkdata ";
	}
	else{
		tagSend.setM_Session();
		if(tag.m_status == (uint8_t)m_SessionModifyBearerRequest){
			std::cout<<"rceive mme  modifybearerrequest ";
			tagSend.setM_SessionModifyBearerRequest();
		}
		else if(tag.m_status == (uint8_t)m_SessionModifyBearerResponse){
			std::cout<<"rceive ugw modifycearerreqsponse ";
			tagSend.setM_SessionModifyBearerResponse();
		}
		packetSend->AddPacketTag(tagSend);
        controllerSendPacket(packetSend,flag);
	}
	std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;


	m_endProcess = Simulator::Now();
	m_mutex.Lock();
	--m_buffCount;
	m_mutex.Unlock();
} 
void controllerApplication::ProcessHandover(lteEpcTag tag,int flag){
	Ptr<Packet> packetSend = Create<Packet>();
	lteEpcTag tagSend;
	Ptr<Packet> packetSend1 = Create<Packet>();
	lteEpcTag tagSend1;
	tagSend.m_count = tag.m_count;
	tagSend.setM_Handover();
	tagSend1.setM_Handover();
	std::cout<<"controller\t: ";
	if(tag.m_status == (uint8_t)m_HandoverModifyBearerRequest){
		std::cout<<"receive Modify Bearer Request from mme";
		tagSend.setM_HandoverModifyBearerRequestToUgw();
		packetSend->AddPacketTag(tagSend);
	    controllerSendPacket(packetSend,flag);
	}
	else if(tag.m_status == (uint8_t)m_HandoverModifyBearerResponseToUgw){
		std::cout<<"receive Modify Bearer Response from ugw";
		tagSend.setM_HandoverModifyBearerResponse();
		tagSend1.setM_HandoverEndMarkerToTargetEnb();
		packetSend1->AddPacketTag(tagSend1);
	    controllerSendPacket(packetSend1,flag);
	}


    std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;
	m_endProcess = Simulator::Now();
	m_mutex.Lock();
	--m_buffCount;
	m_mutex.Unlock();
}
void controllerApplication::ProcessPacket(Ptr<Packet> packet,int flag){
	lteEpcTag tag;
	packet->RemovePacketTag(tag);
	if(tag.m_flag == (uint8_t)m_Session){
		ProcessSession(tag,flag);	
	}
    else if(tag.m_flag == (uint8_t)m_Handover){
        ProcessHandover(tag,flag);
    }

	/*wulei*/
	m_sendCount++;
}
void controllerApplication::RecvFromMmeSocket(Ptr<Socket> socket){
	m_mutex.Lock();
	if(m_buffCount < m_maxBuffSize){
	++m_buffCount;
	Address from;
  	Ptr<Packet> packet = socket->RecvFrom (from);
 	InetSocketAddress m_SocketSourceIp = InetSocketAddress::ConvertFrom (from);

	for(int i=0;i<4;++i){
		if(m_SocketSourceIp.GetIpv4() == m_mmeControllerIfc[i].GetAddress(0)) m_SocketSourceIpFlag = i;
		else if(m_SocketSourceIp.GetIpv4() == m_controllerUgwIfc[i].GetAddress(1)) m_SocketSourceIpFlag = i + 4;
		
	}

    m_revCount++;
	if(m_revCount == 1){
		m_startRev = Simulator::Now();
	}

	Simulator::Schedule(Seconds(0.5),&controllerApplication::ProcessPacket,this,packet,m_SocketSourceIpFlag);
//	ProcessPacket(packet);
	}
	else{
		++m_lossBuff;
	}
	m_mutex.Unlock();
}
void controllerApplication::controllerSendPacket(Ptr<Packet> packetSend,int SocketSourceIpFlag){
//        std::cout << "+++++++++++controllerSendPacket++++++++++" << std::endl;
        switch (SocketSourceIpFlag){
			case 0:
				m_sendToUgwSocket[0]->SendTo(packetSend,0,InetSocketAddress(m_controllerUgwIfc[0].GetAddress(1),m_ugwPort));break;
            case 1:
				m_sendToUgwSocket[1]->SendTo(packetSend,0,InetSocketAddress(m_controllerUgwIfc[1].GetAddress(1),m_ugwPort));break;
            case 2:
				m_sendToUgwSocket[2]->SendTo(packetSend,0,InetSocketAddress(m_controllerUgwIfc[2].GetAddress(1),m_ugwPort));break;
            case 3:
				m_sendToUgwSocket[3]->SendTo(packetSend,0,InetSocketAddress(m_controllerUgwIfc[3].GetAddress(1),m_ugwPort));break;
            case 4:
				m_sendToMmeSocket[0]->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc[0].GetAddress(0),m_mmePort));break;
            case 5:
				m_sendToMmeSocket[1]->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc[1].GetAddress(0),m_mmePort));break;
            case 6:
				m_sendToMmeSocket[2]->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc[2].GetAddress(0),m_mmePort));break;
            case 7:
				m_sendToMmeSocket[3]->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc[3].GetAddress(0),m_mmePort));break;
            default:
                break;
		}

}

}
