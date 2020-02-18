#include "NetMsgUpdateCube.h"

#include <sstream>

#include "GLViewNetTestModule.h"
#include "ManagerGLView.h"

using namespace Aftr;

NetMsgMacroDefinition(NetMsgUpdateCube);

NetMsgUpdateCube::NetMsgUpdateCube()
{
    index = 0;
    translation = Vector(0, 0, 0);
    rotation = Vector(0, 0, 0);
}

bool NetMsgUpdateCube::toStream(NetMessengerStreamBuffer& os) const
{
    os << index;
    os << translation.x << translation.y << translation.z;
    os << rotation.x << rotation.y << rotation.z;
    return true;
}

bool NetMsgUpdateCube::fromStream(NetMessengerStreamBuffer& is)
{
    is >> index;
    is >> translation.x >> translation.y >> translation.z;
    is >> rotation.x >> rotation.y >> rotation.z;
    return true;
}

void NetMsgUpdateCube::onMessageArrived()
{
    // call translateRotateCube in GLView
    static_cast<GLViewNetTestModule*>(ManagerGLView::getGLView())->translateRotateCube(index, translation, rotation, false);
}

std::string NetMsgUpdateCube::toString() const
{
    std::stringstream ss;
    ss << "UpdateCube: ";
    ss << index << " | ";
    ss << translation << " | ";
    ss << rotation;

    return ss.str();
}