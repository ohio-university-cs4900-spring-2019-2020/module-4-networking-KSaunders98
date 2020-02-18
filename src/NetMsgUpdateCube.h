#pragma once

#include "NetMsg.h"
#include "Vector.h"

#ifdef AFTR_CONFIG_USE_BOOST

namespace Aftr {
 // message to update a cube's translation and rotation
 class NetMsgUpdateCube : public NetMsg {
  public:
     NetMsgMacroDeclaration(NetMsgUpdateCube);

     NetMsgUpdateCube();
     virtual bool toStream(NetMessengerStreamBuffer& os) const;
     virtual bool fromStream(NetMessengerStreamBuffer& is);
     virtual void onMessageArrived();
     virtual std::string toString() const;

     unsigned int index;
     Vector translation;
     Vector rotation;
  protected:
 };
}

#endif