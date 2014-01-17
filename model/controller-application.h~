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
#include "ns3/lte-epc-area.h"
#include "ns3/node-buffer.h"
#include "unistd.h"
#include "ns3/system-thread.h"

namespace ns3{

class packFlag{
	Ptr<Packet> m_p;
	int m_IpFlag;	
};
class controllerApplication:public Application{
	public:
		static TypeId GetTypeId(void);
	protected:
		void DoDispose(void);
	public:

//		controllerApplication(Ptr<Node>,Ipv4InterfaceContainer,Ipv4InterfaceContainer,int,int);
                controllerApplication(Ptr<Node>,NodeContainer,NodeContainer,Ipv4InterfaceContainer);

		virtual ~controllerApplication(void);
		virtual void StartApplication();
		virtual void StopApplication();

		void SetStatus(lteEpcTag,uint8_t,uint8_t,Ipv4Address);
		void InstallAreaInfo(std::vector<lteEpcArea>);
		InetSocketAddress getDestinationAddress(Ipv4Address);

		void RecvFromMmeSocket(Ptr<Socket>);
                void RecvFromUgwSocket(Ptr<Socket>);
		void ProcessPacket(Ptr<Packet>,Ipv4Address);
		void ProcessSession(lteEpcTag,Ipv4Address);
		void ProcessHandover(lteEpcTag,Ipv4Address);
		void InitRecvSocket();
		void InitSendSocket();
		void InitSocket();
		void dealPacket();


		Ptr<Node> m_controllerNode;
		NodeContainer m_mmec;
		NodeContainer m_ugwc;

		Ipv4InterfaceContainer m_mmeControllerIfc[4];
		Ipv4InterfaceContainer m_controllerUgwIfc[4];
		Ipv4InterfaceContainer m_ifc;

		int m_mmePort;
		int m_ugwPort;
		int m_port;

		Ptr<Socket> m_socketMme[4];
		Ptr<Socket> m_socketUgw[4];
		Ptr<Socket> m_sendToMmeSocket[4];
		Ptr<Socket> m_sendToUgwSocket[4];
		Ptr<Socket> m_socket;

		std::vector< packFlag > m_vec;
		packFlag m_packflag;
		SystemMutex m_mutex;

		/*wulei*/
		int m_sendCount;	//the numbers of packet that controller send
		int m_revCount;		//the numbers of packet that controller receive
		Time m_startRev;	//the time that controller receive the first packet
		Time m_endProcess;	//the time that controller process the last packet
             
                
		Ptr<SystemThread> m_st[100];
		lteEpcBuffer m_buf;
		int m_threadNum;
		int m_processTime;
		int m_maxsize;

        int m_SocketSourceIpFlag;
//                InetSocketAddress m_SocketSourceIp; //the socket source ip
		int m_maxBuffSize;
		int m_buffCount;
		int m_lossBuff;

//area info 
		std::vector<lteEpcArea> m_area;
};

}
