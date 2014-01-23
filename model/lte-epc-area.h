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
#include "ns3/node-container.h"
#ifndef LTE_EPC_AREA_H
#define LTE_EPC_AREA_H
namespace ns3{
class lteEpcArea{
    public:
	lteEpcArea();
        lteEpcArea(NodeContainer,NodeContainer,NodeContainer,NodeContainer);
        lteEpcArea(NodeContainer,NodeContainer,NodeContainer,NodeContainer,NodeContainer);
        ~lteEpcArea();

        static TypeId GetTypeId();
        void setAreaInfo(NodeContainer,NodeContainer,NodeContainer,NodeContainer);
        void setAreaInfo(NodeContainer,NodeContainer,NodeContainer,NodeContainer,NodeContainer);

        NodeContainer m_uec;
        NodeContainer m_enbc;
        NodeContainer m_mmec;
        NodeContainer m_controllerc;
        NodeContainer m_ugwc;
};
}
#endif
