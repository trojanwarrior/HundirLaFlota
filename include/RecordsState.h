#ifndef RECORDSSTATE_H
#define RECORDSSTATE_H

#define RECORDS_FILENAME "records.xml"

#include <Ogre.h>
#include <OIS/OIS.h>
#include <vector>
#include <xercesc/dom/DOM.hpp>

#include "GameState.h"
#include "GetRecords.h"

class RecordsState : public Ogre::Singleton<RecordsState>, public GameState
{
    private:
      GetRecords *_rec;

    public:
      RecordsState() {}

      void enter ();
      void exit ();
      void pause ();
      void resume ();

      void keyPressed (const OIS::KeyEvent &e);
      void keyReleased (const OIS::KeyEvent &e);

      void mouseMoved (const OIS::MouseEvent &e);
      void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
      void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

      bool frameStarted (const Ogre::FrameEvent& evt);
      bool frameEnded (const Ogre::FrameEvent& evt);
      
      //Quit del GUI
      bool volver(const CEGUI::EventArgs &e);
     
      // Heredados de Ogre::Singleton.
      static RecordsState& getSingleton ();
      static RecordsState* getSingletonPtr ();
      
    protected:
      Ogre::Root* _root;
      Ogre::SceneManager* _sceneMgr;
      Ogre::Viewport* _viewport;
      Ogre::Camera* _camera;
      CEGUI::OgreRenderer* _rendererCEGUI;
      Ogre::Real _tSpeed;
      bool _exitGame;
      bool _volver;
      Ogre::Real _deltaT;
      void createGUI();
      CEGUI::MouseButton convertirBotonMouse(OIS::MouseButtonID id);
      

};

#endif // RECORDSSTATE_H
