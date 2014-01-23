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
#include "ns3/lte-epc-area.h"
using namespace ns3;

int main(int argc,char *argv[]){



	int totalNum = 10;

	double sessionRate = 1.0;
	double handoverRate = 1.0;
	double time = 5;

	CommandLine cmd;
  	cmd.AddValue ("tNum", "int,Number of total users", totalNum);
  	cmd.AddValue ("sRate", "double,sessioinRate", sessionRate);
	cmd.AddValue ("hRate", "double,handoverRate", handoverRate);
	cmd.AddValue ("time", "double,time to send", time);

  	cmd.Parse (argc,argv);

//create ue nodes for four area
   NodeContainer UE;
   UE.Create(6);

   NodeContainer eNB;
   eNB.Create(6);

   NodeContainer MME;
   MME.Create(6);

   NodeContainer Controller;
   Controller.Create(1);

   NodeContainer UGW;
   UGW.Create(6);

   NodeContainer nc;
   nc.Add(UE);
   nc.Add(eNB);
   nc.Add(MME);
   nc.Add(Controller);
   nc.Add(UGW);

//install all node device
   CsmaHelper csma;
   NetDeviceContainer dev;

   dev = csma.Install(nc);

   InternetStackHelper internet;
   internet.Install(nc);
//install ip address for nodes
   Ipv4AddressHelper ipv4;
   ipv4.SetBase("10.0.0.0","255.255.255.0");

   Ipv4InterfaceContainer ifc;
   ifc = ipv4.Assign(dev);

//area ue initial information
   NodeContainer ueArea0;
   ueArea0.Add(UE.Get(0));
   ueArea0.Add(UE.Get(4));
   ueArea0.Add(UE.Get(5));
   NodeContainer ueArea1;
   ueArea1.Add(UE.Get(1));
   NodeContainer ueArea2;
   ueArea2.Add(UE.Get(2));
   NodeContainer ueArea3;
   ueArea3.Add(UE.Get(3));


//area enb initial information
   NodeContainer enbArea0;
   enbArea0.Add(eNB.Get(0));
   enbArea0.Add(eNB.Get(4));
   enbArea0.Add(eNB.Get(5));
   NodeContainer enbArea1;
   enbArea1.Add(eNB.Get(1));
   NodeContainer enbArea2;
   enbArea2.Add(eNB.Get(2));
   NodeContainer enbArea3;
   enbArea3.Add(eNB.Get(3));

//area mme initial information
   NodeContainer mmeArea0;
   mmeArea0.Add(MME.Get(0));
   mmeArea0.Add(MME.Get(4));
   mmeArea0.Add(MME.Get(5));
   NodeContainer mmeArea1;
   mmeArea1.Add(MME.Get(1));
   NodeContainer mmeArea2;
   mmeArea2.Add(MME.Get(2));
   NodeContainer mmeArea3;
   mmeArea3.Add(MME.Get(3));


//area ugw initial information
   NodeContainer ugwArea0;
   ugwArea0.Add(UGW.Get(0));
   ugwArea0.Add(UGW.Get(4));
   ugwArea0.Add(UGW.Get(5));
   NodeContainer ugwArea1;
   ugwArea1.Add(UGW.Get(1));
   NodeContainer ugwArea2;
   ugwArea2.Add(UGW.Get(2));
   NodeContainer ugwArea3;
   ugwArea3.Add(UGW.Get(3));


//install area
   Ptr<LTEHelper> ltehelper ;
   ltehelper = CreateObject<LTEHelper>();

   std::vector<lteEpcArea> vec;
   lteEpcArea area0,area1,area2,area3;
   area0.setAreaInfo(ueArea0,enbArea0,mmeArea0,ugwArea0);
   vec.push_back(area0);
 /*  area1.setAreaInfo(ueArea1,enbArea1,mmeArea1,ugwArea1);
   vec.push_back(area1);
   area2.setAreaInfo(ueArea2,enbArea2,mmeArea2,ugwArea2);
   vec.push_back(area2);
   area3.setAreaInfo(ueArea3,enbArea3,mmeArea3,ugwArea3);
   vec.push_back(area3);
*/
   ltehelper->InstallAreaInfo(vec);

//intial application 


   ltehelper->InstallUe(ueArea0,enbArea0,ifc);
/*   ltehelper->InstallUe(ueArea1,enbArea1,ifc);
   ltehelper->InstallUe(ueArea2,enbArea2,ifc);
   ltehelper->InstallUe(ueArea3,enbArea3,ifc);
*/
   ltehelper->InstalleNB(enbArea0,ueArea0,mmeArea0,ugwArea0,ifc);
/*   ltehelper->InstalleNB(enbArea1,ueArea1,mmeArea1,ugwArea1,ifc);
   ltehelper->InstalleNB(enbArea2,ueArea2,mmeArea2,ugwArea2,ifc);
   ltehelper->InstalleNB(enbArea3,ueArea3,mmeArea3,ugwArea3,ifc);
*/
   ltehelper->InstallMME(mmeArea0,enbArea0,Controller,ifc);
/*   ltehelper->InstallMME(mmeArea1,enbArea1,Controller,ifc);
   ltehelper->InstallMME(mmeArea2,enbArea2,Controller,ifc);
   ltehelper->InstallMME(mmeArea3,enbArea3,Controller,ifc);
*/
   ltehelper->InstallController(Controller,MME,UGW,ifc);

   ltehelper->InstallUGW(ugwArea0,enbArea0,Controller,ifc);
/*   ltehelper->InstallUGW(ugwArea1,enbArea1,Controller,ifc);
   ltehelper->InstallUGW(ugwArea2,enbArea2,Controller,ifc);
   ltehelper->InstallUGW(ugwArea3,enbArea3,Controller,ifc);
*/   
//   ltehelper->InstallSession(UE,ifc);
//   ltehelper->InstallHandover(UE,ifc);

   ltehelper->InstallSession(ueArea0,enbArea0,ifc,totalNum,sessionRate,time);
/*   ltehelper->InstallSession(ueArea1,enbArea1,ifc,totalNum,sessionRate,time);
   ltehelper->InstallSession(ueArea2,enbArea2,ifc,totalNum,sessionRate,time);
   ltehelper->InstallSession(ueArea3,enbArea3,ifc,totalNum,sessionRate,time);
*/

   ltehelper->InstallHandover(enbArea0,mmeArea0,ifc,totalNum,handoverRate,time);
/*   ltehelper->InstallHandover(enbArea1,mmeArea1,ifc,totalNum,handoverRate,time);
   ltehelper->InstallHandover(enbArea2,mmeArea2,ifc,totalNum,handoverRate,time);
   ltehelper->InstallHandover(enbArea3,mmeArea3,ifc,totalNum,handoverRate,time);
*/
   ApplicationContainer app;
   app = ltehelper->GetApplication();
   app.Start(Seconds(0.0));
   app.Stop(Seconds(100000.0));

   Simulator::Run();
   Simulator::Destroy();
   return 0;
   
   

}
