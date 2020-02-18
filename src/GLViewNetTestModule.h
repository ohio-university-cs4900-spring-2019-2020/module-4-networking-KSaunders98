#pragma once

#include "GLView.h"

namespace Aftr
{
   class Camera;
   class NetMessengerClient;

/**
   \class GLViewNetTestModule
   \author Scott Nykl 
   \brief A child of an abstract GLView. This class is the top-most manager of the module.

   Read \see GLView for important constructor and init information.

   \see GLView

    \{
*/

class GLViewNetTestModule : public GLView
{
public:
   static GLViewNetTestModule* New( const std::vector< std::string >& outArgs );
   virtual ~GLViewNetTestModule();
   virtual void updateWorld(); ///< Called once per frame
   virtual void loadMap(); ///< Called once at startup to build this module's scene
   virtual void createNetTestModuleWayPoints();
   virtual void onResizeWindow( GLsizei width, GLsizei height );
   virtual void onMouseDown( const SDL_MouseButtonEvent& e );
   virtual void onMouseDownSelection( unsigned int x, unsigned int y, Camera& cam );
   virtual void onMouseUp( const SDL_MouseButtonEvent& e );
   virtual void onMouseMove( const SDL_MouseMotionEvent& e );
   virtual void onKeyDown( const SDL_KeyboardEvent& key );
   virtual void onKeyUp( const SDL_KeyboardEvent& key );

   void spawnNewCube(bool sendMsg = true);
   void translateRotateCube(unsigned int cube, Vector trans, Vector rot, bool sendMsg = true);

protected:
   GLViewNetTestModule( const std::vector< std::string >& args );
   virtual void onCreate();   

   NetMessengerClient* netClient; // network client
   std::string cubePath; // path to cube model
   std::vector<WO*> cubes; // vector of cubes
   WO* selectedCube; // currently selected cube
   static const double CUBE_DELTA; // amount to translate/rotate cube on each key press
};

/** \} */

} //namespace Aftr
