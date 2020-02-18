#include "GLViewNetTestModule.h"

#include "Axes.h" //We can set Axes to on/off with this
#include "ManagerOpenGLState.h" //We can change OpenGL State attributes with this
#include "NetMessengerClient.h"
#include "PhysicsEngineODE.h"
#include "WorldList.h" //This is where we place all of our WOs

//Different WO used by this module
#include "AftrGLRendererBase.h"
#include "Camera.h"
#include "CameraChaseActorAbsNormal.h"
#include "CameraChaseActorRelNormal.h"
#include "CameraChaseActorSmooth.h"
#include "CameraStandard.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "NetMsgNewCube.h"
#include "NetMsgUpdateCube.h"
#include "WO.h"
#include "WOCar1970sBeater.h"
#include "WOHumanCal3DPaladin.h"
#include "WOHumanCyborg.h"
#include "WOLight.h"
#include "WONVDynSphere.h"
#include "WONVPhysX.h"
#include "WONVStaticPlane.h"
#include "WOSkyBox.h"
#include "WOStatic.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOWayPointSpherical.h"

//If we want to use way points, we need to include this.
#include "NetTestModuleWayPoints.h"

using namespace Aftr;

const double GLViewNetTestModule::CUBE_DELTA = 0.1;

GLViewNetTestModule* GLViewNetTestModule::New(const std::vector<std::string>& args)
{
    GLViewNetTestModule* glv = new GLViewNetTestModule(args);
    glv->init(Aftr::GRAVITY, Vector(0, 0, -1.0f), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE);
    glv->onCreate();
    return glv;
}

GLViewNetTestModule::GLViewNetTestModule(const std::vector<std::string>& args)
    : GLView(args)
{
    //Initialize any member variables that need to be used inside of LoadMap() here.
    //Note: At this point, the Managers are not yet initialized. The Engine initialization
    //occurs immediately after this method returns (see GLViewNetTestModule::New() for
    //reference). Then the engine invoke's GLView::loadMap() for this module.
    //After loadMap() returns, GLView::onCreate is finally invoked.

    //The order of execution of a module startup:
    //GLView::New() is invoked:
    //    calls GLView::init()
    //       calls GLView::loadMap() (as well as initializing the engine's Managers)
    //    calls GLView::onCreate()

    //GLViewNetTestModule::onCreate() is invoked after this module's LoadMap() is completed.
    selectedCube = nullptr;
}

void GLViewNetTestModule::onCreate()
{
    //GLViewNetTestModule::onCreate() is invoked after this module's LoadMap() is completed.
    //At this point, all the managers are initialized. That is, the engine is fully initialized.

    if (this->pe != NULL) {
        //optionally, change gravity direction and magnitude here
        //The user could load these values from the module's aftr.conf
        this->pe->setGravityNormalizedVector(Vector(0, 0, -1.0f));
        this->pe->setGravityScalar(Aftr::GRAVITY);
    }
    this->setActorChaseType(STANDARDEZNAV); //Default is STANDARDEZNAV mode
    //this->setNumPhysicsStepsPerRender( 0 ); //pause physics engine on start up; will remain paused till set to 1

    // connect to localhost on port of other instance
    std::string port = ManagerEnvironmentConfiguration::getVariableValue("NetServerListenPort");
    if (port == "12682") {
        netClient = NetMessengerClient::New("127.0.0.1", "12683");
    } else {
        netClient = NetMessengerClient::New("127.0.0.1", "12682");
    }
}

GLViewNetTestModule::~GLViewNetTestModule()
{
    //Implicitly calls GLView::~GLView()
}

void GLViewNetTestModule::updateWorld()
{
    GLView::updateWorld(); //Just call the parent's update world first.
        //If you want to add additional functionality, do it after
        //this call.
}

void GLViewNetTestModule::onResizeWindow(GLsizei width, GLsizei height)
{
    GLView::onResizeWindow(width, height); //call parent's resize method.
}

void GLViewNetTestModule::onMouseDown(const SDL_MouseButtonEvent& e)
{
    GLView::onMouseDown(e);
}

void GLViewNetTestModule::onMouseDownSelection(unsigned int x, unsigned int y, Camera& cam)
{
    GLView::onMouseDownSelection(x, y, cam);

    WO* selected = getLastSelectedWO();
    if (selected != nullptr && selected->getLabel().substr(0, 4) == "cube") {
        // selected cube, make sure to highlight it and unhighlight previous one
        if (selectedCube != nullptr) {
            selectedCube->getModel()->renderBBox = false;
        }
        selectedCube = selected;
        selectedCube->getModel()->renderBBox = true;
    } else {
        // selected not a cube, make sure to unhighlight previous cube
        if (selectedCube != nullptr) {
            selectedCube->getModel()->renderBBox = false;
            selectedCube = nullptr;
        }
    }
}

void GLViewNetTestModule::onMouseUp(const SDL_MouseButtonEvent& e)
{
    GLView::onMouseUp(e);
}

void GLViewNetTestModule::onMouseMove(const SDL_MouseMotionEvent& e)
{
    GLView::onMouseMove(e);
}

void GLViewNetTestModule::onKeyDown(const SDL_KeyboardEvent& key)
{
    GLView::onKeyDown(key);
    if (key.keysym.sym == SDLK_0)
        this->setNumPhysicsStepsPerRender(1);

    if (key.keysym.sym == SDLK_n) {
        spawnNewCube();
    } else {
        if (selectedCube != nullptr) {
            // translations
            Vector translation = Vector(0, 0, 0);
            if (key.keysym.sym == SDLK_w) {
                translation.x = CUBE_DELTA;
            } else if (key.keysym.sym == SDLK_s) {
                translation.x = -CUBE_DELTA;
            } else if (key.keysym.sym == SDLK_a) {
                translation.y = -CUBE_DELTA;
            } else if (key.keysym.sym == SDLK_d) {
                translation.y = CUBE_DELTA;
            } else if (key.keysym.sym == SDLK_q) {
                translation.z = CUBE_DELTA;
            } else if (key.keysym.sym == SDLK_e) {
                translation.z = -CUBE_DELTA;
            }

            // rotations
            Vector rotation = Vector(0, 0, 0);
            if (key.keysym.sym == SDLK_i) {
                rotation.x = CUBE_DELTA;
            } else if (key.keysym.sym == SDLK_k) {
                rotation.x = -CUBE_DELTA;
            } else if (key.keysym.sym == SDLK_j) {
                rotation.z = -CUBE_DELTA;
            } else if (key.keysym.sym == SDLK_l) {
                rotation.z = CUBE_DELTA;
            } else if (key.keysym.sym == SDLK_u) {
                rotation.y = CUBE_DELTA;
            } else if (key.keysym.sym == SDLK_o) {
                rotation.y = -CUBE_DELTA;
            }

            // get cube index from label
            unsigned int cubeIndex = std::stoi(selectedCube->getLabel().substr(4));

            translateRotateCube(cubeIndex, translation, rotation);
        }
    }
}

void GLViewNetTestModule::onKeyUp(const SDL_KeyboardEvent& key)
{
    GLView::onKeyUp(key);
}

void Aftr::GLViewNetTestModule::loadMap()
{
    this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
    this->actorLst = new WorldList();
    this->netLst = new WorldList();

    ManagerOpenGLState::GL_CLIPPING_PLANE = 1000.0;
    ManagerOpenGLState::GL_NEAR_PLANE = 0.1f;
    ManagerOpenGLState::enableFrustumCulling = false;
    Axes::isVisible = true;
    this->glRenderer->isUsingShadowMapping(true); //set to TRUE to enable shadow mapping, must be using GL 3.2+

    this->cam->setPosition(15, 15, 10);

    cubePath = ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl";
    std::string grass(ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl");

    //SkyBox Textures readily available
    std::vector<std::string> skyBoxImageNames; //vector to store texture paths
    skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_mountains+6.jpg");

    float ga = 0.1f; //Global Ambient Light level for this module
    ManagerLight::setGlobalAmbientLight(aftrColor4f(ga, ga, ga, 1.0f));
    WOLight* light = WOLight::New();
    light->isDirectionalLight(true);
    light->setPosition(Vector(0, 0, 100));
    //Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
    //for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
    light->getModel()->setDisplayMatrix(Mat4::rotateIdentityMat({ 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD));
    light->setLabel("Light");
    worldLst->push_back(light);

    //Create the SkyBox
    WO* wo = WOSkyBox::New(skyBoxImageNames.at(0), this->getCameraPtrPtr());
    wo->setPosition(Vector(0, 0, 0));
    wo->setLabel("Sky Box");
    wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
    worldLst->push_back(wo);

    ////Create the infinite grass plane (the floor)
    wo = WO::New(grass, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
    wo->setPosition(Vector(0, 0, 0));
    wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
    ModelMeshSkin& grassSkin = wo->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);
    grassSkin.getMultiTextureSet().at(0)->setTextureRepeats(5.0f);
    grassSkin.setAmbient(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f)); //Color of object when it is not in any light
    grassSkin.setDiffuse(aftrColor4f(1.0f, 1.0f, 1.0f, 1.0f)); //Diffuse color components (ie, matte shading color of this object)
    grassSkin.setSpecular(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f)); //Specular color component (ie, how "shiney" it is)
    grassSkin.setSpecularCoefficient(10); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
    wo->setLabel("Grass");
    worldLst->push_back(wo);

    createNetTestModuleWayPoints();
}

void GLViewNetTestModule::createNetTestModuleWayPoints()
{
    // Create a waypoint with a radius of 3, a frequency of 5 seconds, activated by GLView's camera, and is visible.
    WayPointParametersBase params(this);
    params.frequency = 5000;
    params.useCamera = true;
    params.visible = true;
    WOWayPointSpherical* wayPt = WOWP1::New(params, 3);
    wayPt->setPosition(Vector(50, 0, 3));
    worldLst->push_back(wayPt);
}

void GLViewNetTestModule::spawnNewCube(bool sendMsg)
{
    // spawn new cube
    WO* wo = WO::New(cubePath, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
    wo->setPosition(Vector(0, 0, 5 * cubes.size() + 3));
    wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;

    std::string label = "cube";
    label += std::to_string(cubes.size());
    wo->setLabel(label);
    worldLst->push_back(wo);
    cubes.push_back(wo);

    if (sendMsg) {
        // send new cube message to other engine instance
        NetMsgNewCube msg;
        netClient->sendNetMsgSynchronousTCP(msg);
    }
}

void GLViewNetTestModule::translateRotateCube(unsigned int cube, Vector trans, Vector rot, bool sendMsg)
{
    if (cube >= cubes.size()) {
        std::cerr << "Tried to translate/rotate cube that doesn't exist!" << std::endl;
        return;
    }

    // apply translation and rotation to cube
    cubes[cube]->moveRelative(trans);
    cubes[cube]->rotateAboutGlobalX(rot.x);
    cubes[cube]->rotateAboutGlobalY(rot.y);
    cubes[cube]->rotateAboutGlobalZ(rot.z);

    if (sendMsg) {
        // send cube update message to other engine instance
        NetMsgUpdateCube msg;
        msg.index = cube;
        msg.translation = trans;
        msg.rotation = rot;
        netClient->sendNetMsgSynchronousTCP(msg);
    }
}