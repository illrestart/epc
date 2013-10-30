
#ifndef SIGNAL_H
#define SIGNAL_H
#include <iostream>
#include "ns3/system-mutex.h"

namespace ns3{

class Signal{
	public:
		Signal();
		~Signal();

		void wait();
		void post();

		SystemMutex m_mutex;
		SystemMutex m_signal;
		int count;
};

}
#endif
