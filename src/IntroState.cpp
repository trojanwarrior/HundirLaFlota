#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
  _camera = _sceneMgr->createCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));
  
  createGui();
  

  _exitGame = false;
  _cambiandoEstado = false;
}

void IntroState::createGui()
{
    
  _rendererCEGUI = &CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::Scheme::setDefaultResourceGroup ("Schemes");
  CEGUI::ImageManager::setImagesetDefaultResourceGroup ("Imagesets");
  CEGUI::Font::setDefaultResourceGroup ("Fonts");
  CEGUI::WindowManager::setDefaultResourceGroup ("Layouts");
  CEGUI::WidgetLookManager::setDefaultResourceGroup ("LookNFeel");

  CEGUI::SchemeManager::getSingleton ().createFromFile ("TaharezLook.scheme");
    
  CEGUI::System::getSingleton ().getDefaultGUIContext ().setDefaultFont ("DejaVuSans-12");
  CEGUI::System::getSingleton ().getDefaultGUIContext ().getMouseCursor ().setDefaultImage ("TaharezLook/MouseArrow");
  
  //PARCHEO RATÓN CEGUI: para que al inicio se encuentre en la misma posicion que el raton de OIS
  // Move CEGUI mouse to (0,0)                                        
  CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(-mousePos.d_x,-mousePos.d_y);
 
  //Sheet
  CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton ().createWindow ("DefaultWindow","HLF/Sheet");
  
  //Marco
  CEGUI::Window* marco = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/FrameWindow");
  marco->setText("Hundir la Flota");
  marco->setSize(CEGUI::USize(CEGUI::UDim(0.45,0), CEGUI::UDim(0.45,0)));
  marco->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.5 - 0.45 / 2, 0), CEGUI::UDim (0.2, 0)));

  CEGUI::Window* onePlayerBtn = CEGUI::WindowManager::getSingleton ().createWindow ("TaharezLook/Button", "HLF/QuitButton");
  onePlayerBtn->setText ("One player vs CPU");
  onePlayerBtn->setSize (CEGUI::USize (CEGUI::UDim (0.50, 0), CEGUI::UDim (0.15, 0)));
  onePlayerBtn->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.5 - 0.5/ 2, 0), CEGUI::UDim (0.1, 0)));
  
  _conexionSubscriptorCegui = onePlayerBtn->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&IntroState::OnePlayer, this));
  
  
  //Quit button
  CEGUI::Window* quitButton = CEGUI::WindowManager::getSingleton ().createWindow ("TaharezLook/Button", "Ex1/QuitButton");
  quitButton->setText ("Quit");
  quitButton->setSize (CEGUI::USize (CEGUI::UDim (0.20, 0), CEGUI::UDim (0.10, 0)));
  quitButton->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.5 - 0.20 / 2, 0), CEGUI::UDim (0.8, 0)));
  quitButton->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&IntroState::quit, this));

  //Attaching buttons
  sheet->addChild(marco);
  marco->addChild(onePlayerBtn);
  marco->addChild(quitButton);
  //sheet->addChild (quitButton);
  CEGUI::System::getSingleton().getDefaultGUIContext ().setRootWindow (sheet);

  
    
}

bool IntroState::OnePlayer(const CEGUI::EventArgs &e)
{
    _cambiandoEstado = true;
    cout << "ha elegido un jugador" << endl;
    _conexionSubscriptorCegui->disconnect();
    changeState(PlayState::getSingletonPtr());
    return true;
}

//Quit del GUI
bool IntroState::quit(const CEGUI::EventArgs &e)
{
  _exitGame = true;
  return true;
}


void IntroState::exit()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
  _rendererCEGUI->destroySystem();  
  
}

void IntroState::pause ()
{
}

void IntroState::resume ()
{
}

bool IntroState::frameStarted(const Ogre::FrameEvent& evt) 
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(evt.timeSinceLastFrame);
  return true;
}

bool IntroState::frameEnded(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void IntroState::keyPressed(const OIS::KeyEvent &e)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(static_cast<CEGUI::Key::Scan>(e.key));
  CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);

  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) {
    changeState(PlayState::getSingletonPtr());
  }
}

void IntroState::keyReleased(const OIS::KeyEvent &e )
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(static_cast<CEGUI::Key::Scan>(e.key));
  CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);

    
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void IntroState::mouseMoved(const OIS::MouseEvent &e)
{
  CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setPosition(CEGUI::Vector2f(e.state.X.abs,e.state.Y.abs));
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(mousePos.d_x/float(e.state.width), mousePos.d_y/float(e.state.height));
  
}

void IntroState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertirBotonMouse(id));
}

void IntroState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    if (!_cambiandoEstado)
        CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertirBotonMouse(id));
}

IntroState* IntroState::getSingletonPtr ()
{
return msSingleton;
}

IntroState& IntroState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

CEGUI::MouseButton IntroState::convertirBotonMouse(OIS::MouseButtonID id)
{
  switch(id)
  {
    case OIS::MB_Right:		cout<< "boton derecho " << endl; return CEGUI::RightButton;
    case OIS::MB_Middle:	cout<< "boton central " << endl; return CEGUI::MiddleButton;
    case OIS::MB_Left:
    default: 			    cout<< "boton izquierdo" << endl; return CEGUI::LeftButton;
  }
}
