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
namespace ns3{

class ugwApplication:public Application{
	public:
		static TypeId GetTypeId(void);
	protected:
		void DoDispose(void);
	public:

		ugwApplication(Ptr<Node>,NodeContainer,NodeContainer,Ipv4InterfaceContainer);
		virtual ~ugwApplication(void);
		virtual void StartApplication();
		virtual void StopApplication();
		void RecvFromEnbSocket(Ptr<Socket>);
                void RecvFromControllerSocket(Ptr<Socket>);

		void SetStatus(lteEpcTag,uint8_t,uint8_t,InetSocketAddress);

		void ProcessPacket(Ptr<Packet>);
		void ProcessSession(lteEpcTag);
		void ProcessHandover(lteEpcTag);
		void InitRecvSocket();
		void InitSendSocket();
		void InitSocket();
		
		Ptr<Node> m_ugwNode;
		NodeContainer m_enbc;
		NodeContainer m_controllerc;

		Ipv4InterfaceContainer m_controllerUgwIfc;
                Ipv4InterfaceContainer m_ugwEnbIfc;
		Ipv4InterfaceContainer m_ifc;

		int m_enbPort;
		int m_controllerPort;
		int m_port;

		Ptr<Socket> m_sendToEnbSocket;
		Ptr<Socket> m_sendToControllerSocket;
		Ptr<Socket> m_socketController;
		Ptr<Socket> m_socketEnb;
		Ptr<Socket> m_socket;

		std::vector< Ptr<Packet> > m_vec;
		SystemMutex m_mutex;

};

}
