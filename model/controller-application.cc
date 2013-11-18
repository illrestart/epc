
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
}

controllerApplication::controllerApplication(Ptr<Node> controllerNode,Ipv4InterfaceContainer mmeControllerIfc[],Ipv4InterfaceContainer controllerUgwIfc[],int mmeport,int ugwport)
	:m_controllerNode(controllerNode),
/*	m_mmeControllerIfc1(mmeControllerIfc.Get(0)),
        m_controllerUgwIfc1(controllerUgwIfc.Get(0)),
	m_mmeControllerIfc2(mmeControllerIfc.Get(1)),
        m_controllerUgwIfc2(controllerUgwIfc.Get(1)),
	m_mmeControllerIfc3(mmeControllerIfc.Get(2)),
        m_controllerUgwIfc3(controllerUgwIfc.Get(2)),
	m_mmeControllerIfc4(mmeControllerIfc.Get(3)),
        m_controllerUgwIfc4(controllerUgwIfc.Get(3)),
*/
	m_mmeControllerIfc1(mmeControllerIfc[0]),
        m_controllerUgwIfc1(controllerUgwIfc[0]),
	m_mmeControllerIfc2(mmeControllerIfc[1]),
        m_controllerUgwIfc2(controllerUgwIfc[1]),
	m_mmeControllerIfc3(mmeControllerIfc[2]),
        m_controllerUgwIfc3(controllerUgwIfc[2]),
	m_mmeControllerIfc4(mmeControllerIfc[3]),
        m_controllerUgwIfc4(controllerUgwIfc[3]),


	m_mmePort(mmeport),
	m_ugwPort(ugwport)
{
	m_port = 8086;

	m_sendCount = 0;
	m_revCount = 0;

	InitSocket();
	

}
controllerApplication::~controllerApplication(void){
	NS_LOG_FUNCTION(this);

}
void controllerApplication::InitRecvSocket(){
	m_socketMme1 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketMme1->Bind(InetSocketAddress(m_mmeControllerIfc1.GetAddress(1),m_port));
	m_socketUgw1 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketUgw1->Bind(InetSocketAddress(m_controllerUgwIfc1.GetAddress(0),m_port));

	m_socketMme2 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketMme2->Bind(InetSocketAddress(m_mmeControllerIfc2.GetAddress(1),m_port));
	m_socketUgw2 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketUgw2->Bind(InetSocketAddress(m_controllerUgwIfc2.GetAddress(0),m_port));

	m_socketMme3 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketMme3->Bind(InetSocketAddress(m_mmeControllerIfc3.GetAddress(1),m_port));
	m_socketUgw3 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketUgw3->Bind(InetSocketAddress(m_controllerUgwIfc3.GetAddress(0),m_port));

	m_socketMme4 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketMme4->Bind(InetSocketAddress(m_mmeControllerIfc4.GetAddress(1),m_port));
	m_socketUgw4 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_socketUgw4->Bind(InetSocketAddress(m_controllerUgwIfc4.GetAddress(0),m_port));
}
void controllerApplication::InitSendSocket(){
	m_sendToMmeSocket1 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToMmeSocket1->Bind(InetSocketAddress(m_mmeControllerIfc1.GetAddress(1),8084));
	m_sendToUgwSocket1 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToUgwSocket1->Bind(InetSocketAddress(m_controllerUgwIfc1.GetAddress(0),8085));

	m_sendToMmeSocket2 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToMmeSocket2->Bind(InetSocketAddress(m_mmeControllerIfc2.GetAddress(1),8084));
	m_sendToUgwSocket2 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToUgwSocket2->Bind(InetSocketAddress(m_controllerUgwIfc2.GetAddress(0),8085));
	
	m_sendToMmeSocket3 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToMmeSocket3->Bind(InetSocketAddress(m_mmeControllerIfc3.GetAddress(1),8084));
	m_sendToUgwSocket3 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToUgwSocket3->Bind(InetSocketAddress(m_controllerUgwIfc3.GetAddress(0),8085));

	m_sendToMmeSocket4 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToMmeSocket4->Bind(InetSocketAddress(m_mmeControllerIfc4.GetAddress(1),8084));
	m_sendToUgwSocket4 = Socket::CreateSocket(m_controllerNode,TypeId::LookupByName("ns3::UdpSocketFactory"));
	m_sendToUgwSocket4->Bind(InetSocketAddress(m_controllerUgwIfc4.GetAddress(0),8085));
}
void controllerApplication::InitSocket(){
	InitRecvSocket();

	InitSendSocket();
}
void controllerApplication::StartApplication(){
	NS_LOG_FUNCTION(this);	
	m_socketMme1->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));
	m_socketUgw1->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));

	m_socketMme2->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));
	m_socketUgw2->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));

	m_socketMme3->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));
	m_socketUgw3->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));

	m_socketMme4->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));
	m_socketUgw4->SetRecvCallback(MakeCallback(&controllerApplication::RecvFromMmeSocket,this));

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
//		m_sendToUgwSocket->SendTo(packetSend,0,InetSocketAddress(m_controllerUgwIfc.GetAddress(1),8086));
                controllerSendPacket(packetSend);
	}
	else if(tag.m_status == (uint8_t)m_SessionModifyBearerResponse){
		std::cout<<"rceive ugw modifycearerreqsponse ";
		tagSend.setM_Session();
		tagSend.setM_SessionModifyBearerResponse();
		packetSend->AddPacketTag(tagSend);
//		m_sendToMmeSocket->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc.GetAddress(0),m_mmePort));
                controllerSendPacket(packetSend);
	}
	std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;

//        std::cout << "ProcessSession m_SocketSourceIpFlag = " << m_SocketSourceIpFlag << std::endl;

	m_endProcess = Simulator::Now();
} 
void controllerApplication::ProcessHandover(lteEpcTag tag){
	Ptr<Packet> packetSend = Create<Packet>();
	lteEpcTag tagSend;
	Ptr<Packet> packetSend1 = Create<Packet>();
	lteEpcTag tagSend1;
	tagSend.m_count = tag.m_count;
	std::cout<<"controller\t: ";
	if(tag.m_status == (uint8_t)m_HandoverModifyBearerRequest){
		std::cout<<"receive Modify Bearer Request from mme";
                tagSend.setM_Handover();
		tagSend.setM_HandoverModifyBearerRequestToUgw();
		packetSend->AddPacketTag(tagSend);
//		m_sendToUgwSocket->SendTo(packetSend,0,InetSocketAddress(m_controllerUgwIfc.GetAddress(1),8086));
                controllerSendPacket(packetSend);
	}
	else if(tag.m_status == (uint8_t)m_HandoverModifyBearerResponseToUgw){
		std::cout<<"receive Modify Bearer Response from ugw";
		tagSend.setM_Handover();
		tagSend.setM_HandoverModifyBearerResponse();
		packetSend->AddPacketTag(tagSend);
//		m_sendToMmeSocket->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc.GetAddress(0),m_mmePort));
                controllerSendPacket(packetSend);

		tagSend1.setM_Handover();
		tagSend1.setM_HandoverEndMarkerToTargetEnb();
		packetSend1->AddPacketTag(tagSend1);
//		m_sendToMmeSocket->SendTo(packetSend1,0,InetSocketAddress(m_mmeControllerIfc.GetAddress(0),m_mmePort));
                controllerSendPacket(packetSend1);
	}
        std::cout<<"\t:tag number"<<tag.m_count<<"----------"<<Simulator::Now().GetSeconds()<<std::endl;
//        std::cout << "PacketHandover m_SocketSourceIpFlag = " << m_SocketSourceIpFlag << std::endl;
	m_endProcess = Simulator::Now();
}
void controllerApplication::ProcessPacket(Ptr<Packet> packet){
	lteEpcTag tag;
	packet->RemovePacketTag(tag);
	if(tag.m_flag == (uint8_t)m_Session){
		ProcessSession(tag);	
	}
        else if(tag.m_flag == (uint8_t)m_Handover){
                ProcessHandover(tag);
        }

	/*wulei*/
	m_sendCount++;
}
void controllerApplication::RecvFromMmeSocket(Ptr<Socket> socket){
        
	Address from;
  	Ptr<Packet> packet = socket->RecvFrom (from);
 	InetSocketAddress m_SocketSourceIp = InetSocketAddress::ConvertFrom (from);

  	if(m_SocketSourceIp.GetIpv4() == m_mmeControllerIfc1.GetAddress(0))
       		m_SocketSourceIpFlag = 0;
   	if(m_SocketSourceIp.GetIpv4() == m_controllerUgwIfc1.GetAddress(1))
       		m_SocketSourceIpFlag = 1;
  
  	if(m_SocketSourceIp.GetIpv4() == m_mmeControllerIfc2.GetAddress(0))
       		m_SocketSourceIpFlag = 2;
  	if(m_SocketSourceIp.GetIpv4() == m_controllerUgwIfc2.GetAddress(1))
       		m_SocketSourceIpFlag = 3;

  	if(m_SocketSourceIp.GetIpv4() == m_mmeControllerIfc3.GetAddress(0))
       		m_SocketSourceIpFlag = 4;
  	if(m_SocketSourceIp.GetIpv4() == m_controllerUgwIfc3.GetAddress(1))
       		m_SocketSourceIpFlag = 5;

  	if(m_SocketSourceIp.GetIpv4() == m_mmeControllerIfc4.GetAddress(0))
       		m_SocketSourceIpFlag = 6;
  	if(m_SocketSourceIp.GetIpv4() == m_controllerUgwIfc4.GetAddress(1))
       		m_SocketSourceIpFlag = 7;

//        std::cout << "the socket source IP is : "<< m_SocketSourceIp.GetIpv4() << std::endl;
//        std::cout << "m_SocketSourceIpFlag = " << m_SocketSourceIpFlag << std::endl;

        m_revCount++;
	if(m_revCount == 1){
		m_startRev = Simulator::Now();
	}

	Simulator::Schedule(Seconds(0.0),&controllerApplication::ProcessPacket,this,packet);

}


void controllerApplication::controllerSendPacket(Ptr<Packet> packetSend){
//        std::cout << "+++++++++++controllerSendPacket++++++++++" << std::endl;
        switch (m_SocketSourceIpFlag){
                case 0:
                   m_sendToUgwSocket1->SendTo(packetSend,0,InetSocketAddress(m_controllerUgwIfc1.GetAddress(1),m_ugwPort));break;
                case 1:
   		   m_sendToMmeSocket1->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc1.GetAddress(0),m_mmePort));break;
                case 2:
       		   m_sendToUgwSocket2->SendTo(packetSend,0,InetSocketAddress(m_controllerUgwIfc2.GetAddress(1),m_ugwPort));break;
                case 3:
   		   m_sendToMmeSocket2->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc2.GetAddress(0),m_mmePort));break;
                case 4:
       		   m_sendToUgwSocket3->SendTo(packetSend,0,InetSocketAddress(m_controllerUgwIfc3.GetAddress(1),m_ugwPort));break;
                case 5:
   		   m_sendToMmeSocket3->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc3.GetAddress(0),m_mmePort));break;
                case 6:
       		   m_sendToUgwSocket4->SendTo(packetSend,0,InetSocketAddress(m_controllerUgwIfc4.GetAddress(1),m_ugwPort));break;
                case 7:
   		   m_sendToMmeSocket4->SendTo(packetSend,0,InetSocketAddress(m_mmeControllerIfc4.GetAddress(0),m_mmePort));break;
                default:
                   break;
}

}

}
