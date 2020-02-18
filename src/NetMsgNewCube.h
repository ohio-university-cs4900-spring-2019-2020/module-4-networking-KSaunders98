#pragma once

#include "NetMsg.h"

#ifdef AFTR_CONFIG_USE_BOOST

namespace Aftr {
 // message for creating a new cube
 class NetMsgNewCube : public NetMsg {
  public:
     NetMsgMacroDeclaration(NetMsgNewCube);

     virtual bool toStream(NetMessengerStreamBuffer& os) const;
     virtual bool fromStream(NetMessengerStreamBuffer& is);
     virtual void onMessageArrived();
     virtual std::string toString() const;
 protected:
 };
}

#endif