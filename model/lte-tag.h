#include <iostream>
#include "ns3/tag.h"
#include "ns3/ptr.h"
#include "ns3/uinteger.h"
#include "ns3/application.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/traced-callback.h"
#include "ns3/object.h"
#include "ns3/callback.h"

#ifndef LTEEPCTAG_H
#define LTEEPCTAG_H
namespace ns3{
enum Flag{
		m_Session,
		m_Handover,
};
enum Signal_Status{
	//		int m_paraNum;
			/*
			*the flag of session or handover
			*/
		m_Empty,
			
			/*
			*session and handover procedure
			*/
		m_SessionServiceRequest,
		m_SessionAuthentication,
		m_SessionInitialContextSetupRequest,
		m_SessionRadioBearerEstablishment,
		m_SessionUplinkData,
		m_SessionInitialContextSetupComplete,
		m_SessionModifyBearerRequest,
		m_SessionModifyBearerRespone,
		m_SessionPCEFInitialedIPCAN,
		m_SessionModifyBearerResponse,

		m_HandoverPathSwitchRequest,
		m_HandoverModifyBearerRequest,
		m_HandoverModifyBearerRequestToUgw,
		m_HandoverModifyBearerResponseToUgw,
		m_HandoverModifyBearerResponse,
		m_HandoverEndMarkerToSourceEnb,
		m_HandoverEndMarkerToTargetEnb,
		m_HandoverPathSwitchRequestAck,
		m_HandoverReleaseResource
};

class lteEpcTag : public Tag {
	public:
		lteEpcTag();
		~lteEpcTag();

		static TypeId GetTypeId(void);

		virtual TypeId GetInstanceTypeId(void) const;

		virtual uint32_t GetSerializedSize(void) const;

		virtual void Serialize(TagBuffer) const;

		virtual void Deserialize(TagBuffer);
		
		virtual void Print(std::ostream &os)const{}

		void setM_Session();
		void setM_Handover();

		void setM_SessionServiceRequest();
		void setM_SessionAuthentication();
		void setM_SessionInitialContextSetupRequest();
		void setM_SessionRadioBearerEstablishment();
		void setM_SessionUplinkData();
		void setM_SessionInitialContextSetupComplete();
		void setM_SessionModifyBearerRequest();
		void setM_SessionModifyBearerRespone();
		void setM_SessionPCEFInitialedIPCAN();
		void setM_SessionModifyBearerResponse();

		void setM_HandoverPathSwitchRequest();
		void setM_HandoverModifyBearerRequest();
		void setM_HandoverModifyBearerRequestToUgw();
		void setM_HandoverModifyBearerResponseToUgw();
		void setM_HandoverModifyBearerResponse();
		void setM_HandoverEndMarkerToSourceEnb();
		void setM_HandoverEndMarkerToTargetEnb();
		void setM_HandoverPathSwitchRequestAck();
		void setM_HandoverReleaseResource();
		
		uint8_t m_status;
		uint8_t m_flag;
		int m_count;
		int m_id;

};

}
#endif
