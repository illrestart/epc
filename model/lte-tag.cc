#include "lte-tag.h"

NS_LOG_COMPONENT_DEFINE("lteEpcTag");

namespace ns3{

TypeId lteEpcTag::GetTypeId(void){
	static TypeId tid = TypeId("ns3::lteEpcTag")
		.SetParent<Tag>()
		.AddConstructor<lteEpcTag>()
		;
	return tid;
}

TypeId lteEpcTag::GetInstanceTypeId(void) const{
	return GetTypeId();
}

uint32_t lteEpcTag::GetSerializedSize(void) const{
	return 4;
}
void lteEpcTag::Serialize(TagBuffer i) const{
	i.WriteU8(m_flag);
	i.WriteU8(m_status);
	i.WriteU8(m_count);
	i.WriteU8(m_id);
}

void lteEpcTag::Deserialize(TagBuffer i){
	m_flag = i.ReadU8();
	m_status = i.ReadU8();
	m_count = i.ReadU8();
	m_id = i.ReadU8();
	
}

//void lteEpcTag::Print(std::ostream &os){}

lteEpcTag::lteEpcTag(){
	NS_LOG_INFO(this);
/*
*the flag of session or handover
*/
	m_status = (uint8_t)m_Empty;
	
}

lteEpcTag::~lteEpcTag(){
	NS_LOG_INFO(this);
}
/*
*the flag of session or handover
*/
void lteEpcTag::setM_Session() {
		m_flag = (uint8_t)m_Session;
}

void lteEpcTag::setM_Handover() {
		m_flag = (uint8_t)m_Handover;
}



/*
*session and handover set 
*/

void lteEpcTag::setM_SessionServiceRequest() {
		m_status = (uint8_t)m_SessionServiceRequest;
}
void lteEpcTag::setM_SessionAuthentication() {
	m_status = (uint8_t)m_SessionAuthentication;
}
void lteEpcTag::setM_SessionInitialContextSetupRequest() {
	m_status = (uint8_t)m_SessionInitialContextSetupRequest;
}
void lteEpcTag::setM_SessionRadioBearerEstablishment() {
	m_status = (uint8_t)m_SessionRadioBearerEstablishment;
}
void lteEpcTag::setM_SessionUplinkData() {
	m_status = (uint8_t)m_SessionUplinkData;
}
void lteEpcTag::setM_SessionInitialContextSetupComplete() {
	m_status = (uint8_t)m_SessionInitialContextSetupComplete;
}
void lteEpcTag::setM_SessionModifyBearerRequest() {
	m_status = (uint8_t)m_SessionModifyBearerRequest;
}
void lteEpcTag::setM_SessionModifyBearerRespone() {
	m_status = (uint8_t)m_SessionModifyBearerRespone;
}
void lteEpcTag::setM_SessionPCEFInitialedIPCAN() {
	m_status = (uint8_t)m_SessionPCEFInitialedIPCAN;
}
void lteEpcTag::setM_SessionModifyBearerResponse() {
	m_status = (uint8_t)m_SessionModifyBearerResponse;
}
void lteEpcTag::setM_HandoverPathSwitchRequest() {
	m_status = (uint8_t)m_HandoverPathSwitchRequest;
}
void lteEpcTag::setM_HandoverModifyBearerRequest() {
	m_status = (uint8_t)m_HandoverModifyBearerRequest;
}
void lteEpcTag::setM_HandoverModifyBearerRequestToUgw() {
	m_status = (uint8_t)m_HandoverModifyBearerRequestToUgw;
}
void lteEpcTag::setM_HandoverModifyBearerResponseToUgw() {
	m_status = (uint8_t)m_HandoverModifyBearerResponseToUgw;
}
void lteEpcTag::setM_HandoverModifyBearerResponse() {
	m_status = (uint8_t)m_HandoverModifyBearerResponse;
}
void lteEpcTag::setM_HandoverEndMarkerToSourceEnb() {
	m_status = (uint8_t)m_HandoverEndMarkerToSourceEnb;
}
void lteEpcTag::setM_HandoverEndMarkerToTargetEnb() {
	m_status = (uint8_t)m_HandoverEndMarkerToTargetEnb;
}
void lteEpcTag::setM_HandoverPathSwitchRequestAck() {
	m_status = (uint8_t)m_HandoverPathSwitchRequestAck;
}
void lteEpcTag::setM_HandoverReleaseResource() {
	m_status = (uint8_t)m_HandoverReleaseResource;
}


}
