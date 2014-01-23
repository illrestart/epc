#include "ltehelper.h"
#include <ns3/log.h>
#include <string.h>
#include <cstring>
#include "ns3/InitialModelHelper.h"

NS_LOG_COMPONENT_DEFINE("LTEHelper");
//using namespace std;

namespace ns3{


NS_OBJECT_ENSURE_REGISTERED(LTEHelper);
LTEHelper::LTEHelper()
{}
LTEHelper::LTEHelper(int totalNum,float sessionRate,float handoverRate,float time)
	:m_totalNumber(totalNum),
	m_sessionRate(sessionRate),
	m_handoverRate(handoverRate),
	m_time(time)
{
	NS_LOG_FUNCTION(this);
}

LTEHelper::~LTEHelper()
{
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

void LTEHelper::InstallUe(NodeContainer uec,NodeContainer enbc,Ipv4InterfaceContainer ifc){
    for(uint32_t i = 0; i < uec.GetN(); ++i){
        Ptr<ueApplication> ueapp = CreateObject<ueApplication>(uec.Get(i),enbc,ifc);
	uec.Get(i)->AddApplication(ueapp);
        m_app.Add(ueapp);
    }
}

void LTEHelper::InstalleNB(NodeContainer enbc,NodeContainer uec,NodeContainer mmec,NodeContainer ugwc,Ipv4InterfaceContainer ifc){
    for(uint32_t i = 0; i < enbc.GetN(); ++i){
        Ptr<enbApplication> enbapp = CreateObject<enbApplication>(enbc.Get(i),enbc,uec,mmec,ugwc,ifc);
	enbc.Get(i)->AddApplication(enbapp);
        m_app.Add(enbapp);
    }
}

void LTEHelper::InstallMME(NodeContainer mmec,NodeContainer enbc,NodeContainer controllerc,Ipv4InterfaceContainer ifc){
    for(uint32_t i = 0; i < mmec.GetN(); ++i){
        Ptr<mmeApplication> mmeapp = CreateObject<mmeApplication>(mmec.Get(i),mmec,enbc,controllerc,ifc);
	mmec.Get(i)->AddApplication(mmeapp);
        m_app.Add(mmeapp);
    }
}


void LTEHelper::InstallController(NodeContainer controllerc,NodeContainer mmec,NodeContainer ugwc,Ipv4InterfaceContainer ifc){
        Ptr<controllerApplication> controllerapp = CreateObject<controllerApplication>(controllerc.Get(0),mmec,ugwc,ifc);
	controllerc.Get(0)->AddApplication(controllerapp);
	controllerapp->InstallAreaInfo(m_area);
        m_app.Add(controllerapp);
}

void LTEHelper::InstallUGW(NodeContainer ugwc,NodeContainer enbc,NodeContainer controllerc,Ipv4InterfaceContainer ifc){
    for(uint32_t i = 0; i < ugwc.GetN(); ++i){
        Ptr<ugwApplication> ugwapp = CreateObject<ugwApplication>(ugwc.Get(i),enbc,controllerc,ifc);
	ugwc.Get(i)->AddApplication(ugwapp);
        m_app.Add(ugwapp);
    }
}

void LTEHelper::InstallSession(NodeContainer uec,NodeContainer enbc,Ipv4InterfaceContainer ifc,int tNum,float rate,float time){
    for(uint32_t i = 0; i < uec.GetN(); ++i){	
    	Ptr<SessionApplication> sessionapp = CreateObject<SessionApplication>(uec.Get(i),enbc,ifc,tNum,rate,time);
	uec.Get(i)->AddApplication(sessionapp);
    	m_app.Add(sessionapp);
    }
}
void LTEHelper::InstallHandover(NodeContainer enbc,NodeContainer mmec,Ipv4InterfaceContainer ifc,int tNum,float rate,float time){
    for(uint32_t i = 0; i < enbc.GetN(); ++i){
    	Ptr<HandoverApplication> handoverapp = CreateObject<HandoverApplication>(enbc.Get(i),mmec,ifc,tNum,rate,time);
	enbc.Get(i)->AddApplication(handoverapp);
    	m_app.Add(handoverapp);
    }
}
void LTEHelper::InstallAreaInfo(std::vector<lteEpcArea> vec){
    m_area = vec;   
}
ApplicationContainer LTEHelper::GetApplication(){
    return m_app;
}
}