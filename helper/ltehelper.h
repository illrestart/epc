#include <iostream>
#include "ns3/object.h"
#include "ns3/ipv4.h"
#include "ns3/socket.h"
#include "ns3/application.h"
#include "ns3/application-container.h"
#include "ns3/node.h"
#include "ns3/ipv4-address.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/address.h"
#include "ns3/network-module.h"
#include "ns3/core-module.h"
#include "ns3/netanim-module.h"
#include "ns3/ue-application.h"
#include "ns3/enb-application.h"
#include "ns3/mme-application.h"
#include "ns3/controller-application.h"
#include "ns3/ugw-application.h"
#include "ns3/point-to-point-module.h"
#ifndef LTEHELPER_H
#define LTEHELPER_H
namespace ns3{
	class LTEHelper:public Object{
		public:
			LTEHelper(int,int,float,float,float);
			virtual ~LTEHelper();
	
			static TypeId GetTypeId();

			ApplicationContainer InstallApplication(Ptr<Node>,Ptr<Node>,Ptr<Node>,Ptr<Node>,Ptr<Node>);

			ApplicationContainer InstallTotalApplication();
			ApplicationContainer InstallNodeApplication();
			ApplicationContainer GetNodeApplication();
			void SetNodePosition();
			void InstallPosition(int);
			void CreateNode(int);
			void InstallNodeDevice(int);
			void InstallNodeAddress(int);
			void InstallNodeApplication(int);
			void InstallIpBase();

			void InstallPort();
			void InstallStack();

			NodeContainer GetUeNode();
			Ipv4InterfaceContainer GetUeToEnbIp(int);

			Ptr<ueApplication> InstallSingleUe(Ptr<Node>,Ipv4InterfaceContainer);
			Ptr<enbApplication> InstallSingleEnb(Ptr<Node>,Ipv4InterfaceContainer,Ipv4InterfaceContainer,Ipv4InterfaceContainer);
			Ptr<mmeApplication> InstallSingleMme(Ptr<Node>,Ipv4InterfaceContainer,Ipv4InterfaceContainer);
//			Ptr<controllerApplication> InstallSingleController(Ptr<Node>,Ipv4InterfaceContainer,Ipv4InterfaceContainer);
                        Ptr<controllerApplication> InstallSingleController(Ptr<Node>,Ipv4InterfaceContainer mmeAddress[],Ipv4InterfaceContainer ugwAddress[]);
                        Ptr<ugwApplication> InstallSingleUgw(Ptr<Node>,Ipv4InterfaceContainer,Ipv4InterfaceContainer);

			Ipv4AddressHelper ipv4h[20];

			float ue_x[4];
			float ue_y[4];
			float enb_x[4];
			float enb_y[4];
			float mme_x[4];
			float mme_y[4];
			float ugw_x[4];
			float ugw_y[4];

			int m_uePort;
			int m_enbPort;
			int m_mmePort;
			int m_controllerPort;
			int m_ugwPort;


			CsmaHelper csma;
			ApplicationContainer m_app;

			NodeContainer m_ue;
			NodeContainer m_enb;
			NodeContainer m_mme;
			NodeContainer m_controller;
			NodeContainer m_ugw;

			NetDeviceContainer dev[4];
			NetDeviceContainer dev1[4];
			NetDeviceContainer dev2[4];
			NetDeviceContainer dev3[4];
			NetDeviceContainer dev4[4];
	
			Ipv4InterfaceContainer ifc[4];
			Ipv4InterfaceContainer ifc1[4];
			Ipv4InterfaceContainer ifc2[4];
			Ipv4InterfaceContainer ifc3[4];
			Ipv4InterfaceContainer ifc4[4];

/*wulei*/
			int m_num;				//the number of area
			int m_totalNumber;			//total user number,the unit is 10000
			float m_sessionRate;			//the users to send session packet in 10000 every second
			float m_handoverRate;			//the users to send handover packet in 10000 every second
			float m_time;				//packet send for time

                        
//                        InetSocketAddress m_SocketSourceIp; //the socket source ip
	
	};
}
#endif
