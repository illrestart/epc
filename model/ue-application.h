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
#include "ns3/system-thread.h"
#include "ns3/system-mutex.h"
#include "signal.h"
#include "ns3/internet-module.h"
namespace ns3{

class ueApplication:public Application{
	public:
		static TypeId GetTypeId(void);
	protected:
		void DoDispose(void);
	public:

		ueApplication(Ptr<Node>,NodeContainer,Ipv4InterfaceContainer);
		virtual ~ueApplication(void);
		virtual void StartApplication();
		virtual void StopApplication();
		void SetStatus(lteEpcTag,uint8_t,uint8_t,InetSocketAddress);

		void RecvFromEnbSocket(Ptr<Socket>);

		void ProcessPacket(Ptr<Packet>);
		void ProcessSession(lteEpcTag);
		void InitRecvSocket();
		void InitSendSocket();
		void InitSocket();

		Ptr<Node> m_ueNode;
		NodeContainer m_enbc;
		Ipv4InterfaceContainer m_ifc;
		int m_enbPort;
		int m_port;
		Ptr<Socket> m_socket;
		Ptr<Socket> m_sendToEnbSocket;
		Ptr<Socket> m_socketEnb;

		std::vector< Ptr<Packet> > m_vec;
		SystemMutex m_mutex;
		Signal m_signal;
};

}
