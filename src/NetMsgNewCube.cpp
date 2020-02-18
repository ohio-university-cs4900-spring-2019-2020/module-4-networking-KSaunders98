#include "NetMsgNewCube.h"

#include "ManagerGLView.h"
#include "GLViewNetTestModule.h"

using namespace Aftr;

NetMsgMacroDefinition(NetMsgNewCube);

bool NetMsgNewCube::toStream(NetMessengerStreamBuffer& os) const
{
    return true;
}

bool NetMsgNewCube::fromStream(NetMessengerStreamBuffer& is)
{
    return true;
}

void NetMsgNewCube::onMessageArrived()
{
    // call spawnNewCube in GLView
    static_cast<GLViewNetTestModule*>(ManagerGLView::getGLView())->spawnNewCube(false);
}

std::string NetMsgNewCube::toString() const {
    return "NewCube";
}