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
#include "vector"
#include "ns3/lte-epc-area.h"
#include "ns3/internet-module.h"
#include "ns3/node-container.h"
#include "ns3/ipv4-interface-container.h"
#ifndef LTEHELPER_H
#define LTEHELPER_H
namespace ns3{
	class LTEHelper:public Object{
		public:
			LTEHelper();
			LTEHelper(int,float,float,float);
			virtual ~LTEHelper();
	
			static TypeId GetTypeId();

            void InstallUe(NodeContainer,NodeContainer,Ipv4InterfaceContainer);
            void InstalleNB(NodeContainer,NodeContainer,NodeContainer,NodeContainer,Ipv4InterfaceContainer);
            void InstallMME(NodeContainer,NodeContainer,NodeContainer,Ipv4InterfaceContainer);
            void InstallController(NodeContainer,NodeContainer,NodeContainer,Ipv4InterfaceContainer);
            void InstallUGW(NodeContainer,NodeContainer,NodeContainer,Ipv4InterfaceContainer);

            void InstallSession(NodeContainer,NodeContainer,Ipv4InterfaceContainer,int,float,float);
            void InstallHandover(NodeContainer,NodeContainer,Ipv4InterfaceContainer,int,float,float);
            void InstallAreaInfo(std::vector<lteEpcArea>);

            ApplicationContainer GetApplication();

            ApplicationContainer m_app;
            std::vector<lteEpcArea> m_area;

            int m_totalNumber;
            float m_sessionRate;
            float m_handoverRate;
            float m_time;

    };
}
#endif
