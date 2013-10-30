#include "signal.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("Signal");
namespace ns3{
Signal::Signal(){
	NS_LOG_FUNCTION(this);
	count = 0;
}
Signal::~Signal(){
	NS_LOG_FUNCTION(this);
}

void Signal::wait(){
	if( count > 0 ){
		m_mutex.Lock();
		count--;
		m_mutex.Unlock();
	}
	else if(count == 0){
		m_signal.Lock();
	}
	else if(count < 0){
		std::cout<<"error count < 0\n";
	}
}
void Signal::post(){
	if(count == 0){
		m_mutex.Lock();
		count++;
		m_mutex.Unlock();
		m_signal.Unlock();
	}
	else if(count > 0){
		m_mutex.Lock();
		count++;
		m_mutex.Unlock();
	}
	else if(count < 0){
		std::cout<<"error \n";
	}
}
}
