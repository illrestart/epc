#include <iostream>
#include "ns3/object.h"
#include "ns3/ipv4.h"
#include "ns3/socket.h"
#include "ns3/application.h"
#include "ns3/application-container.h"
#include "ns3/node.h"
#include "ns3/ipv4-address.h"
#include "ns3/session-application.h"
#include "ns3/inet-socket-address.h"

#ifndef INITIALMODELHELPER_H
#define INITIALMODELHELPER_H

namespace ns3{

class InitialModelHelper: public Object{
	public:
		InitialModelHelper();
		virtual ~InitialModelHelper();

		static TypeId GetTypeId();

		ApplicationContainer InstallModelSession(Ptr<Node>,Ipv4Address,Ipv4Address);
};

}
#endif
