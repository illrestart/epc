#include "ns3/address.h"
#include "ns3/socket.h"
#include "ns3/traced-callback.h"
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/application.h"
#include "lte-tag.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "ns3/system-mutex.h"
#include "signal.h"
#include "ns3/internet-module.h"
namespace ns3{

class enbApplication:public Application{
	public:
		static TypeId GetTypeId(void);
	protected:
		void DoDispose(void);
	public:

		enbApplication(Ptr<Node>,NodeContainer,NodeContainer,NodeContainer,NodeContainer,Ipv4InterfaceContainer);
		virtual ~enbApplication(void);
		virtual void StartApplication();
		virtual void StopApplication();
		void SetStatus(lteEpcTag,uint8_t,uint8_t,InetSocketAddress);

		void RecvFromUeSocket(Ptr<Socket>);
                void RecvFromMmeSocket(Ptr<Socket>);
                void RecvFromUgwSocket(Ptr<Socket>);

		void ProcessPacket(Ptr<Packet>);
		void ProcessSession(lteEpcTag);
		void ProcessHandover(lteEpcTag);
		void InitRecvSocket();
		void InitSendSocket();
		void InitSocket();

		void SendInitialContextSetupComplete(Ptr<Socket>);

		Ptr<Node> m_enbNode;
		NodeContainer m_mmec;
		NodeContainer m_ugwc;

		Ipv4InterfaceContainer m_ueEnbIfc;
		Ipv4InterfaceContainer m_enbMmeIfc;
		Ipv4InterfaceContainer m_ugwEnbIfc;
		Ipv4InterfaceContainer m_ifc;

		int m_port;
		Ptr<Socket> m_socket;
		Ptr<Socket> m_sendToUeSocket;
		Ptr<Socket> m_sendToMmeSocket;
		Ptr<Socket> m_sendToUgwSocket;
		Ptr<Socket> m_socketUe;
		Ptr<Socket> m_socketMme;
		Ptr<Socket> m_socketUgw;
		int m_uePort;
		int m_mmePort;
		int m_ugwPort;

		std::vector< Ptr<Packet> > m_vec;
		SystemMutex m_mutex;
		Signal m_signal;
};

}
