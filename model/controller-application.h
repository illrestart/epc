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

class controllerApplication:public Application{
	public:
		static TypeId GetTypeId(void);
	protected:
		void DoDispose(void);
	public:

		controllerApplication(Ptr<Node>,Ipv4InterfaceContainer,int,Ipv4InterfaceContainer,int);
		virtual ~controllerApplication(void);
		virtual void StartApplication();
		virtual void StopApplication();
		void RecvFromMmeSocket(Ptr<Socket>);
                void RecvFromUgwSocket(Ptr<Socket>);
		void ProcessPacket(Ptr<Packet>);
		void ProcessSession(lteEpcTag);
		void InitRecvSocket();
		void InitSendSocket();
		void InitSocket();

		Ptr<Node> m_controllerNode;

		Ipv4InterfaceContainer m_mmeControllerIfc;
                Ipv4InterfaceContainer m_controllerUgwIfc;
		int m_mmePort;
		int m_ugwPort;
		int m_port;

		Ptr<Socket> m_sendToMmeSocket;
		Ptr<Socket> m_sendToUgwSocket;
		Ptr<Socket> m_socketMme;
		Ptr<Socket> m_socketUgw;

		std::vector< Ptr<Packet> > m_vec;
		SystemMutex m_mutex;
};

}
