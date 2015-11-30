#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  //RenderWindow *_renderWindow = _root->initialise(true,"Hundir la flota");
  //_renderer = _root->initialise(true,"Hundir la flota");
  _sceneMgr = _root->createSceneManager (ST_INTERIOR,"SceneManager");
  _sceneMgr->setAmbientLight (Ogre::ColourValue (1, 1, 1));



  _camera = _sceneMgr->createCamera ("IntroCamera");
  //_camera->setPosition (Vector3 (0.72, 11.0, -20.37));
  _camera->setPosition (Vector3 (0.0, 11.0, -20.37));
  _camera->lookAt (Vector3 (-3.45, 1.45, 2.69));
  _camera->setNearClipDistance (0.1);
  _camera->setFarClipDistance (100);


  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  //_viewport = _renderWindow->addViewport (_camera);
  double width = _viewport->getActualWidth ();
  double height = _viewport->getActualHeight ();
  _camera->setAspectRatio (width / height);
  _viewport->setBackgroundColour (ColourValue (0.0, 0.0, 0.0));

  //Tiempo entre frame y frame
  _deltaT = 0;
  
  //Para que el barco se mueva mecido por las aguas (que poético :D)
  _mecer = -10;


  //Creamos la escena
  createScene ();
  //Gui
  createGUI ();
 
  _exitGame = false;
  _tSpeed = 10.0;                              //Distancia en unidades del mundo virtual que queremos recorrer en un segundo cuando movamos cositas

}

void
IntroState::createGUI ()
{
  _renderer = &CEGUI::OgreRenderer::bootstrapSystem ();
  CEGUI::Scheme::setDefaultResourceGroup ("Schemes");
  CEGUI::ImageManager::setImagesetDefaultResourceGroup ("Imagesets");
  CEGUI::Font::setDefaultResourceGroup ("Fonts");
  CEGUI::WindowManager::setDefaultResourceGroup ("Layouts");
  CEGUI::WidgetLookManager::setDefaultResourceGroup ("LookNFeel");

  CEGUI::SchemeManager::getSingleton ().createFromFile ("TaharezLook.scheme");
  CEGUI::System::getSingleton ().getDefaultGUIContext ().setDefaultFont ("DejaVuSans-12");
  CEGUI::System::getSingleton ().getDefaultGUIContext ().getMouseCursor ().setDefaultImage ("TaharezLook/MouseArrow");

  //Sheet
  CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton ().createWindow ("DefaultWindow","Ex1/Sheet");

  //Quit button
  CEGUI::Window* quitButton = CEGUI::WindowManager::getSingleton ().createWindow ("TaharezLook/Button", "Ex1/QuitButton");
  quitButton->setText ("Quit");
  quitButton->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.05, 0)));
  quitButton->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.5 - 0.15 / 2, 0), CEGUI::UDim (0.2, 0)));
  quitButton->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&IntroState::quit, this));

  //Attaching buttons
  sheet->addChild (quitButton);
  CEGUI::System::getSingleton ().getDefaultGUIContext ().setRootWindow (sheet);

}

void
IntroState::createScene ()
{
  StaticGeometry *tablero = _sceneMgr->createStaticGeometry ("Tablero");
  Entity *entTablero = _sceneMgr->createEntity ("tablero.mesh");
  tablero->addEntity (entTablero, Vector3 (0, 0, 0)); // Añadir la entidad a tablero siempre antes de llamar a build()
  tablero->build ();

  //Objeto movable "suelo" para consultar al sceneManager
  SceneNode *nodeTableroCol = _sceneMgr->createSceneNode ("tableroCol");
  Entity *entTableroCol = _sceneMgr->createEntity ("entTableroCol", "tableroCol.mesh");
  entTableroCol->setQueryFlags (STAGE);
  nodeTableroCol->attachObject (entTableroCol);
  nodeTableroCol->setVisible (false);
  _sceneMgr->getRootSceneNode ()->addChild (nodeTableroCol);

  //Casillas invisibles para hacer las consultas por RayQuery
  stringstream sauxnode;
  string s = "Casilla_col";
  for (int i = 0; i < 100; i++)
  {
    sauxnode << s << i;
    SceneNode *nodeCasillaCol = _sceneMgr->createSceneNode (sauxnode.str ());
    Entity *entCasillaCol = _sceneMgr->createEntity (sauxnode.str (), "CasillaCol.mesh");
    entCasillaCol->setQueryFlags (STAGE);
    nodeCasillaCol->attachObject (entCasillaCol);
    nodeCasillaCol->setVisible (false);
    nodeTableroCol->addChild (nodeCasillaCol);
    sauxnode.str ("");
  }

  // Comentario referente a la exportación de Blender a Ogre:
  // Hay que tener cuidado a la hora de exportar las figuras y tener claro
  // lo que se exporta. OgreToBlender exportará sólo la figura seleccionada
  // y sus materiales (texturas) asociados. Por lo que es recomendable renombrar
  // tanto los nombres de las figuras como de sus materiales asociados de lo
  // contrario sobreescribirá aquellos que se llamen igual. Lo cual puede estar
  // bien si pretendemos usar los mismos materiales para distintas figuras.
  SceneNode *nodoBarquito = _sceneMgr->createSceneNode ("Barquito");
  Entity *entBarquito = _sceneMgr->createEntity ("barquito", "barquito.mesh");
  nodoBarquito->attachObject (entBarquito);
  _sceneMgr->getRootSceneNode ()->addChild (nodoBarquito);

  SceneNode *nodoLuz = _sceneMgr->createSceneNode ("Luces");
  _sceneMgr->setShadowTechnique (SHADOWTYPE_STENCIL_ADDITIVE);
  Light *luz = _sceneMgr->createLight ("Luz1");
  luz->setType (Light::LT_SPOTLIGHT);
  luz->setPosition (9, 2, 5);
  luz->setDirection (Vector3 (3, -2, 0));
  luz->setSpotlightInnerAngle (Degree (5.0f));
  luz->setSpotlightOuterAngle (Degree (55.0f));
  luz->setSpotlightFalloff (0.0f);
  nodoLuz->attachObject (luz);
  _sceneMgr->getRootSceneNode ()->addChild (nodoLuz);

}

//Quit del GUI
bool IntroState::quit (const CEGUI::EventArgs &e)
{
  _exitGame = true;
  return true;
}



void IntroState::exit()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause ()
{
}

void IntroState::resume ()
{
}

bool IntroState::frameStarted(const Ogre::FrameEvent& evt)
{
  _deltaT = evt.timeSinceLastFrame; //Tiempo transcurrido desde que se pinto el último frame
  CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(_deltaT);

  //int fps = 1.0 / _deltaT;                              //Para calcular el rendimiento

  _camera->moveRelative(_vtCamara * _deltaT * _tSpeed);
  if (_camera->getPosition().length() < 10.0) {
    _camera->moveRelative(-_vtCamara * _deltaT * _tSpeed);
  }

  _sceneMgr->getSceneNode("Barquito")->translate(_vtBarco * _deltaT * _tSpeed, Ogre::Node::TS_PARENT);
  _sceneMgr->getSceneNode("Barquito")->yaw(Ogre::Degree(_r * _deltaT));

  _mecer = _mecer % 10;
  std::cout << _mecer << endl;
  if (!_mecer)
      _mecer *= -1;
  //_sceneMgr->getSceneNode("Barquito")->roll(Ogre::Degree(Ogre::Math::Sin((_mecer * _deltaT),true)));
  _sceneMgr->getSceneNode("Barquito")->roll(Ogre::Degree(_mecer * _deltaT));
  _mecer++;
  
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

  
  _vtBarco = Ogre::Vector3(0,0,0);
  _vtCamara = Ogre::Vector3(0,0,0);


  switch (e.key)
  {
    case OIS::KC_UP:		_vtCamara+=Ogre::Vector3(0,0,-1); cout<< "UP" << endl; break;
    case OIS::KC_DOWN:		_vtCamara+=Ogre::Vector3(0,0,1);  cout<< "DOWN" << endl; break;
    case OIS::KC_LEFT:		_vtCamara+=Ogre::Vector3(-1,0,0); cout<< "LEFT" << endl; break;
    case OIS::KC_RIGHT:		_vtCamara+=Ogre::Vector3(1,0,0);  cout<< "RIGHT" << endl; break;
    case OIS::KC_A:	_vtBarco+=Vector3(1,0,0);  break;
    case OIS::KC_S:	_vtBarco+=Vector3(0,0,-1); break;
    case OIS::KC_D:	_vtBarco+=Vector3(-1,0,0); break;
    case OIS::KC_W:	_vtBarco+=Vector3(0,0,1);  break;
    default:   	cout << "VTBarco " << _vtBarco << endl;
		cout << "VTCamara " << _vtCamara << endl;

  }

  
  _r = 0;
  if(e.key == OIS::KC_R) _r+=180;

  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) 
     changeState(PlayState::getSingletonPtr());

}

void IntroState::keyReleased(const OIS::KeyEvent &e )
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(static_cast<CEGUI::Key::Scan>(e.key));
  
  _vtBarco = Ogre::Vector3(0,0,0);
  _vtCamara = Ogre::Vector3(0,0,0);
  _r = 0;
  
  if (e.key == OIS::KC_ESCAPE)
     _exitGame = true;
    
}

void IntroState::mouseMoved(const OIS::MouseEvent &e)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(e.state.X.rel, e.state.Y.rel);  
}

void IntroState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  //mbLeft = _mouse->getMouseState().buttonDown(OIS::MB_Left);
   //mbMiddle = _mouse->getMouseState().buttonDown(OIS::MB_Middle);
   //mbRight =  _mouse->getMouseState().buttonDown(OIS::MB_Right);

  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertirBotonMouse(id));

   if (id == OIS::MB_Middle)
   {
     cout << "se ha presionado el boton central" << endl;
     //float rotx = _mouse->getMouseState().X.rel * deltaT * -1;
     //float roty = _mouse->getMouseState().Y.rel * deltaT * -1;
     float rotx = e.state.X.rel * _deltaT * -1;
     float roty = e.state.Y.rel * _deltaT * -1;
     
     _camera->yaw(Radian(rotx));
     _camera->pitch(Radian(roty));
   }

}

void IntroState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
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
