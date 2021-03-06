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

class mmeApplication:public Application{
	public:
		static TypeId GetTypeId(void);
	protected:
		void DoDispose(void);
	public:
		mmeApplication(Ptr<Node>,Ipv4InterfaceContainer,int,Ipv4InterfaceContainer,int);
		virtual ~mmeApplication(void);
		virtual void StartApplication();
		virtual void StopApplication();
		void RecvFromEnbSocket(Ptr<Socket>);
		void RecvFromControllerSocket(Ptr<Socket>);

		void ProcessPacket(Ptr<Packet>);
		void ProcessSession(lteEpcTag);

		void InitRecvSocket();
		void InitSendSocket();
		void InitSocket();

		Ptr<Node> m_mmeNode;

		Ipv4InterfaceContainer m_enbMmeIfc;
		Ipv4InterfaceContainer m_mmeControllerIfc;
		int m_enbPort;
		int m_controllerPort;
		int m_port;
		Ptr<Socket> m_sendToEnbSocket;
		Ptr<Socket> m_sendToControllerSocket;
		Ptr<Socket> m_socketEnb;
		Ptr<Socket> m_socketController;

		std::vector< Ptr<Packet> > m_vec;
		SystemMutex m_mutex;
		Signal m_signal;
};

}
