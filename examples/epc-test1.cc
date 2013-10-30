#include <ns3/ltehelper.h>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/config-store.h"
#include <iostream>
#include "ns3/csma-module.h"
#include "ns3/lte-tag.h"
#include "ns3/packet.h"
#include "ns3/socket.h"
#include "ns3/inet-socket-address.h"
#include "ns3/address.h"
#include "ns3/InitialModelHelper.h"
#include "ns3/point-to-point-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("epc-test1");

int main(){
	LogComponentEnable("epc-test1",LOG_LEVEL_INFO);

	std::string AnimFile = "epc-test1";


	int AreaNum = 4;

	Ptr<LTEHelper> ltehelper = CreateObject<LTEHelper>();
	ApplicationContainer app;
	app = ltehelper->InstallTotalApplication(AreaNum);
	app.Start(Seconds(0.0));
//	app.Stop(Seconds(30.0));

	NodeContainer ue = ltehelper->GetUeNode();
	ApplicationContainer app1;
	ApplicationContainer appc;
	InitialModelHelper inithelp;
	Ipv4InterfaceContainer ipv4AddressContainer;

	for(int i = 0; i < AreaNum ; ++i){
		ipv4AddressContainer = ltehelper->GetUeToEnbIp(i);

		app1 = inithelp.InstallModelSession(ue.Get(i),ipv4AddressContainer.GetAddress(0),ipv4AddressContainer.GetAddress(1));
		appc.Add(app1);
	}
	appc.Start(Seconds(0.0));
//	appc.Start(Seconds(30.0));

	ltehelper->InstallPosition(AreaNum);

	Simulator::Run();
	Simulator::Destroy();
	return 0;

}
