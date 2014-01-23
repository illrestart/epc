#include "lte-epc-area.h"


NS_LOG_COMPONENT_DEFINE("lteEpcArea");

namespace ns3{

TypeId lteEpcArea::GetTypeId(void){
	static TypeId tid = TypeId("ns3::lteEpcArea")
		//.SetParent<Tag>()
		//.AddConstructor<lteEpcTag>()
		;
	return tid;
}
lteEpcArea::lteEpcArea()
{
}
lteEpcArea::lteEpcArea(NodeContainer ue,NodeContainer enb,NodeContainer mme,NodeContainer ugw):
    m_uec(ue),
    m_enbc(enb),
    m_mmec(mme),
    m_ugwc(ugw)
{
	NS_LOG_INFO(this);
}
lteEpcArea::lteEpcArea(NodeContainer ue,NodeContainer enb,NodeContainer mme,NodeContainer controller,NodeContainer ugw):
    m_uec(ue),
    m_enbc(enb),
    m_mmec(mme),
    m_controllerc(controller),
    m_ugwc(ugw)
{
	NS_LOG_INFO(this);
}
   
lteEpcArea::~lteEpcArea(){
	NS_LOG_INFO(this);
}
void lteEpcArea::setAreaInfo(NodeContainer ue,NodeContainer enb,NodeContainer mme,NodeContainer ugw){
    m_uec = ue;
    m_enbc = enb;
    m_mmec = mme;
    m_ugwc = ugw;
}
void lteEpcArea::setAreaInfo(NodeContainer ue,NodeContainer enb,NodeContainer mme,NodeContainer controller,NodeContainer ugw){
    m_uec = ue;
    m_enbc = enb;
    m_mmec = mme;
    m_controllerc = controller;
    m_ugwc = ugw;
}
}
