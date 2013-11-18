#include "ns3/address.h"
#include "ns3/socket.h"
#include "ns3/traced-callback.h"
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/application.h"
#include "lte-tag.h"
#include "ns3/inet-socket-address.h"
#include "ns3/event-id.h"
#include "ns3/data-rate.h"


namespace ns3{

class SessionApplication:public Application{
	public:
		static TypeId GetTypeId(void);
	protected:
		void DoDispose(void);
	public:

		SessionApplication(Ptr<Node>,InetSocketAddress,InetSocketAddress,int,float,float);
		virtual ~SessionApplication(void);
		
		virtual void StartApplication();
		virtual void StopApplication();
	

		void CancelEvents();
		void ScheduleStartEvent();
		void StartSending();
		void sendPacket();

		Ptr<Node> m_node;
		InetSocketAddress m_remote;
		InetSocketAddress m_local;
		Ptr<Socket> m_socket;
		Time m_lastStartTime;
		EventId m_startSendEvent;
		float tmpTime;				//trace the time seted to send packet

		int m_totalNumber;			//total user number,the unit is 10000
		float m_rate;				//the users to send packet in 10000 every second
		float m_interval;			//request =1 / (m_totalNumber * m_rate)
		float m_time;				//the time that packet to send
		bool m_flag;				//if false, it will close session process

		int sessionCount;
};

	

}
