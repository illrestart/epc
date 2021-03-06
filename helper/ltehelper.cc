#include "ltehelper.h"
#include <ns3/log.h>
#include <string.h>
#include <cstring>

NS_LOG_COMPONENT_DEFINE("LTEHelper");

namespace ns3{


NS_OBJECT_ENSURE_REGISTERED(LTEHelper);

LTEHelper::LTEHelper(){
	NS_LOG_FUNCTION(this);

	InstallPort();
	SetNodePosition();

	InstallIpBase();
//	csma.SetChannelAttribute("DataRate",DataRateValue(DataRate("10Gb/s")));
//	csma.SetChannelAttribute("Delay",TimeValue(Seconds(0.1)));
//	csma.SetDeviceAttribute("Mtu",UintegerValue(1000));
}

LTEHelper::~LTEHelper(){
	NS_LOG_FUNCTION(this);
}


TypeId LTEHelper::GetTypeId(){
	static TypeId tid = 
		TypeId("ns3::LTEHelper")
		.SetParent<Object>()
		;
		//.AddConstructor<LTEHelper>()
		//;
	return tid;
}
void LTEHelper::InstallIpBase(){
	char ip[9] = "10.0.0.0";
	char ip1[10] = "10.10.0.0";
	for(int i = 0 ; i < 20 ; ++i){
		if(i < 10){
			ip[3] = i + '0';
			std::cout<<ip<<std::endl;
			ipv4h[i].SetBase(ip,"255.255.0.0");
		}
		else if(i >= 10){
			ip1[4] = i%10 + '0';
			std::cout<<ip1<<std::endl;
			ipv4h[i].SetBase(ip1,"255.255.0.0");
		}
	}
}
NodeContainer LTEHelper::GetUeNode(){
	return m_ue;
}
Ipv4InterfaceContainer LTEHelper::GetUeToEnbIp(int num){
	return ifc[num]; 
}
void LTEHelper::SetNodePosition(){
	double uex[4]={10.0,190.0,10.0,190.0};
	double uey[4]={150.0,150.0,50.0,50.0};
	double enbx[4]={30.0,170.0,30.0,170.0};
	double enby[4]={150.0,150.0,50.0,50.0};
	double mmex[4]={30.0,170.0,30.0,170.0};
	double mmey[4]={120.0,120.0,80.0,80.0};
	double ugwx[4]={50.0,150.0,50.0,150.0};
	double ugwy[4]={150.0,150.0,50.0,50.0};

	for(int i = 0; i < 4 ; ++i){
		ue_x[i] = uex[i];
		ue_y[i] = uey[i];
		enb_x[i] = enbx[i];
		enb_y[i] = enby[i];
		mme_x[i] = mmex[i];
		mme_y[i] = mmey[i];
		ugw_x[i] = ugwx[i];
		ugw_y[i] = ugwy[i];
	}
}
void LTEHelper::InstallPosition(int num){
	AnimationInterface::SetConstantPosition(m_controller.Get(0),100.0,100.0);
	for(int i = 0; i < num ;++i){
		
		AnimationInterface::SetConstantPosition(m_ue.Get(i),ue_x[i],ue_y[i]);
		AnimationInterface::SetConstantPosition(m_enb.Get(i),enb_x[i],enb_y[i]);
		AnimationInterface::SetConstantPosition(m_mme.Get(i),mme_x[i],mme_y[i]);
		AnimationInterface::SetConstantPosition(m_ugw.Get(i),ugw_x[i],ugw_y[i]);
	}
}
void LTEHelper::InstallPort(){
	m_uePort = 8086;
	m_enbPort = 8086;
	m_mmePort = 8086;
	m_controllerPort = 8086;
	m_ugwPort = 8086;
}
void LTEHelper::InstallStack(){
	InternetStackHelper stack;
	stack.Install(m_ue);
	stack.Install(m_enb);
	stack.Install(m_mme);
	stack.Install(m_controller);
	stack.Install(m_ugw);
}
ApplicationContainer LTEHelper::InstallTotalApplication(int num){
	CreateNode(num);
	return GetNodeApplication();
}
void LTEHelper::CreateNode(int num){
	m_ue.Create(num);
	m_enb.Create(num);
	m_mme.Create(num);
	m_controller.Create(1);
	m_ugw.Create(num);
	InstallStack();
	InstallNodeDevice(num);
}
void LTEHelper::InstallNodeDevice(int num){
	for(int i = 0; i < num ;++i){
		dev[i]  = csma.Install(NodeContainer(m_ue.Get(i),m_enb.Get(i)));
		dev1[i] = csma.Install(NodeContainer(m_enb.Get(i),m_mme.Get(i)));
		dev2[i] = csma.Install(NodeContainer(m_mme.Get(i),m_controller.Get(0)));
		dev3[i] = csma.Install(NodeContainer(m_controller.Get(0),m_ugw.Get(i)));
		dev4[i] = csma.Install(NodeContainer(m_ugw.Get(i),m_enb.Get(i)));
		InstallNodeAddress(i);
	}
}
void LTEHelper::InstallNodeAddress(int area){
	ifc[area]  = ipv4h[4*area + 0].Assign(dev[area]);
	ifc1[area] = ipv4h[4*area + 1].Assign(dev1[area]);
	ifc2[area] = ipv4h[4*area + 2].Assign(dev2[area]);
	ifc3[area] = ipv4h[4*area + 3].Assign(dev3[area]);
	ifc4[area] = ipv4h[4*area + 4].Assign(dev4[area]);

	InstallNodeApplication(area);
}
void LTEHelper::InstallNodeApplication(int area){
	Ptr<ueApplication> ueapp = InstallSingleUe(m_ue.Get(area),ifc[area]);
	Ptr<enbApplication> enbapp = InstallSingleEnb(m_enb.Get(area),ifc[area],ifc1[area],ifc4[area]);
	Ptr<mmeApplication> mmeapp = InstallSingleMme(m_mme.Get(area),ifc1[area],ifc2[area]);
	Ptr<controllerApplication> controllerapp = InstallSingleController(m_controller.Get(0),ifc2[area],ifc3[area]);
	Ptr<ugwApplication> ugwapp = InstallSingleUgw(m_ugw.Get(area),ifc3[area],ifc4[area]);

	m_ue.Get(area)->AddApplication(ueapp);
	m_enb.Get(area)->AddApplication(enbapp);
	m_mme.Get(area)->AddApplication(mmeapp);
	m_controller.Get(0)->AddApplication(controllerapp);
	m_ugw.Get(area)->AddApplication(ugwapp);

	m_app.Add(ueapp);
	m_app.Add(enbapp);
	m_app.Add(mmeapp);
	m_app.Add(controllerapp);
	m_app.Add(ugwapp);
}
ApplicationContainer LTEHelper::GetNodeApplication(){
	return m_app;
}
Ptr<ueApplication> LTEHelper::InstallSingleUe(Ptr<Node> ue,Ipv4InterfaceContainer enbAddress){
	Ptr<ueApplication> ueapp = CreateObject<ueApplication>(ue,enbAddress,m_enbPort); 
	return ueapp;
}
 Ptr<enbApplication> LTEHelper::InstallSingleEnb(Ptr<Node> enb,Ipv4InterfaceContainer ueAddress,Ipv4InterfaceContainer mmeAddress,Ipv4InterfaceContainer ugwAddress){
	Ptr<enbApplication> enbapp = CreateObject<enbApplication>(enb,ueAddress,m_uePort,mmeAddress,m_mmePort,ugwAddress,m_ugwPort); 
	return enbapp;
}
 Ptr<mmeApplication> LTEHelper::InstallSingleMme(Ptr<Node> mme,Ipv4InterfaceContainer enbAddress,Ipv4InterfaceContainer controllerAddress){
	Ptr<mmeApplication> mmeapp = CreateObject<mmeApplication>(mme,enbAddress,m_enbPort,controllerAddress,m_controllerPort); 
	return mmeapp;
}
 Ptr<controllerApplication> LTEHelper::InstallSingleController(Ptr<Node> controller,Ipv4InterfaceContainer mmeAddress,Ipv4InterfaceContainer ugwAddress){
	Ptr<controllerApplication> controllerapp = CreateObject<controllerApplication>(controller,mmeAddress,m_mmePort,ugwAddress,m_ugwPort); 
	return controllerapp;
}
 Ptr<ugwApplication> LTEHelper::InstallSingleUgw(Ptr<Node> ugw,Ipv4InterfaceContainer enbAddress,Ipv4InterfaceContainer controllerAddress){
	Ptr<ugwApplication> ugwapp = CreateObject<ugwApplication>(ugw,enbAddress,m_enbPort,controllerAddress,m_controllerPort); 
	return ugwapp;
}

}


