#include "ns3/address.h"
#include "ns3/socket.h"
#include "ns3/traced-callback.h"
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/application.h"
#include "lte-tag.h"
#include "ns3/system-mutex.h"
#include "vector"
#include "ns3/internet-module.h"
#include "ns3/packet.h"
#include "ns3/network-module.h"
namespace ns3{

class controllerApplication:public Application{
	public:
		static TypeId GetTypeId(void);
	protected:
		void DoDispose(void);
	public:

//		controllerApplication(Ptr<Node>,Ipv4InterfaceContainer,Ipv4InterfaceContainer,int,int);
                controllerApplication(Ptr<Node> controllerNode,Ipv4InterfaceContainer mmeControllerIfc[],Ipv4InterfaceContainer controllerUgwIfc[],int,int);

		virtual ~controllerApplication(void);
		virtual void StartApplication();
		virtual void StopApplication();
		void RecvFromMmeSocket(Ptr<Socket>);
                void RecvFromUgwSocket(Ptr<Socket>);
		void ProcessPacket(Ptr<Packet>);
		void ProcessSession(lteEpcTag);
		void ProcessHandover(lteEpcTag);
		void InitRecvSocket();
		void InitSendSocket();
		void InitSocket();

                void controllerSendPacket(Ptr<Packet>); //controller send packet to MME or UGW

		Ptr<Node> m_controllerNode;

		Ipv4InterfaceContainer m_mmeControllerIfc1;
                Ipv4InterfaceContainer m_controllerUgwIfc1;
		Ipv4InterfaceContainer m_mmeControllerIfc2;
                Ipv4InterfaceContainer m_controllerUgwIfc2;
		Ipv4InterfaceContainer m_mmeControllerIfc3;
                Ipv4InterfaceContainer m_controllerUgwIfc3;
		Ipv4InterfaceContainer m_mmeControllerIfc4;
                Ipv4InterfaceContainer m_controllerUgwIfc4;

		int m_mmePort;
		int m_ugwPort;
		int m_port;

		Ptr<Socket> m_sendToMmeSocket1;
		Ptr<Socket> m_sendToUgwSocket1;
		Ptr<Socket> m_socketMme1;
		Ptr<Socket> m_socketUgw1;
		Ptr<Socket> m_sendToMmeSocket2;
		Ptr<Socket> m_sendToUgwSocket2;
		Ptr<Socket> m_socketMme2;
		Ptr<Socket> m_socketUgw2;
		Ptr<Socket> m_sendToMmeSocket3;
		Ptr<Socket> m_sendToUgwSocket3;
		Ptr<Socket> m_socketMme3;
		Ptr<Socket> m_socketUgw3;
		Ptr<Socket> m_sendToMmeSocket4;
		Ptr<Socket> m_sendToUgwSocket4;
		Ptr<Socket> m_socketMme4;
		Ptr<Socket> m_socketUgw4;

		std::vector< Ptr<Packet> > m_vec;
		SystemMutex m_mutex;

		/*wulei*/
		int m_sendCount;	//the numbers of packet that controller send
		int m_revCount;		//the numbers of packet that controller receive
		Time m_startRev;	//the time that controller receive the first packet
		Time m_endProcess;	//the time that controller process the last packet
             
                
                int m_SocketSourceIpFlag;
//                InetSocketAddress m_SocketSourceIp; //the socket source ip
};

}
