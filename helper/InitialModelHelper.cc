#include "InitialModelHelper.h"
#include "ns3/log.h"
#include "ns3/string.h"

NS_LOG_COMPONENT_DEFINE("InitialModelHelper");

namespace ns3{

NS_OBJECT_ENSURE_REGISTERED(InitialModelHelper);

InitialModelHelper::InitialModelHelper(){
	NS_LOG_FUNCTION(this);
}

InitialModelHelper::~InitialModelHelper(){
	NS_LOG_FUNCTION(this);
}

TypeId InitialModelHelper::GetTypeId(){
	static TypeId tid = 
		TypeId("ns3::InitialModelHelper")
		.SetParent<Object>()
		;
		//.AddConstructor<LTEHelper>()
		//;
	return tid;
}


Ptr<SessionApplication> InitialModelHelper::InstallModelSession(Ptr<Node> ue,Ipv4Address localAddress,Ipv4Address remoteAddress,int totalNumber,float rate,float time){

		Ptr<SessionApplication> session = CreateObject<SessionApplication>(ue,InetSocketAddress(localAddress,8083),InetSocketAddress(remoteAddress,8086),totalNumber,rate,time);
//		ue->AddApplication(session);
		return session;
	}

Ptr<HandoverApplication> InitialModelHelper::InstallModelHandover(Ptr<Node> ue,Ipv4Address localAddress,Ipv4Address remoteAddress,int totalNumber,float rate,float time){
		
		Ptr<HandoverApplication> handover = CreateObject<HandoverApplication>(ue,InetSocketAddress(localAddress,8083),InetSocketAddress(remoteAddress,8086),totalNumber,rate,time);
//		ue->AddApplication(handover);
		return handover;
			
	}
}
