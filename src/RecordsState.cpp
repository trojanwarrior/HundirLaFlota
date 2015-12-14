#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <iostream>
#include <boost/filesystem.hpp>
#include "RecordsState.h"


template<> RecordsState* Ogre::Singleton<RecordsState>::msSingleton = 0;

void RecordsState::enter()
{
  _root = Ogre::Root::getSingletonPtr();
  _sceneMgr = _root->getSceneManager("SceneManager");
  _sceneMgr->setAmbientLight (Ogre::ColourValue (1, 1, 1));
  _camera = _sceneMgr->getCamera ("IntroCamera");

  //_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  double width = _viewport->getActualWidth ();
  double height = _viewport->getActualHeight ();
  _camera->setAspectRatio (width / height);
  _viewport->setBackgroundColour (ColourValue (0.0, 0.0, 0.0));

  //Tiempo entre frame y frame
  _deltaT = 0;
  
  //Instanciamos objeto para obtener los records desde un xml
  try
  {
    _rec = new GetRecords();
    
  }
  catch (...)
  {
      cout << "Whop!! ¿Qué habrá pasado? \n";
  }
  
  //Creamos la escena
  //createScene ();
  //Gui
  createGUI ();
  
  _camera->setPosition (Vector3 (0,12,17));
  //_camera->lookAt (_sceneMgr->getSceneNode("Barquito")->getPosition());
  _camera->yaw(Degree(0));
  _camera->pitch(Degree(0));
  _camera->roll(Degree(0));

  //_camera->lookAt (Vector3 (-3.45, 1.45, 2.69));
  _camera->setNearClipDistance (0.1);
  _camera->setFarClipDistance (100);
 
  _exitGame = false;
  _volver = false;
  _tSpeed = 10.0;                              //Distancia en unidades del mundo virtual que queremos recorrer en un segundo cuando movamos cositas

}

void RecordsState::createGUI()
{
  //_rendererCEGUI = &CEGUI::OgreRenderer::bootstrapSystem();
  //_rendererCEGUI = CEGUI::System::getSingleton().getDefaultGUIContext();
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
  
  //Config Window
  CEGUI::Window* marcoRecords = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("records.layout");
  
  
  
  //Setting Text!
  try
  {
      stringstream aux;
      aux << "./media/" << RECORDS_FILENAME;
      string a = aux.str();
      _rec->readRecords(a); //Genera el vector de records. Si no lo llamamos primero nunca tendremos records
  }
  catch (std::exception &e)
  {
      cout << "el fichero de records probablemente no existe, habrá que jugar para que se creen nuevos records. \n";
  }
  
  if (!_rec->Records().empty())
  {
      std::vector<record>::iterator it;
      //int i=1;
      //for (it = _rec->Records().begin(); it!=_rec->Records().end(); ++it)
      for (size_t i=0; i<_rec->Records().size(); i++)
      {
              std::stringstream s,p;
              s << "Text" << i;
              //p << it[i].posicion << ". " << it[i].nombre << "            " << it[i].puntuacion;
              p << _rec->Records()[i].nombre << "[horz-alignment='right']" << _rec->Records()[i].puntuacion;
              marcoRecords->getChild(s.str())->setText(p.str());
      }    
  }

  marcoRecords->getChild("ExitButton")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&RecordsState::volver, this));
  //Quit button
  /*
  CEGUI::Window* quitButton = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "HLF/QuitButton");
  quitButton->setText ("Quit");
  quitButton->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.05, 0)));
  quitButton->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.1 - 0.15 / 2, 0), CEGUI::UDim (0.1, 0)));
  quitButton->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&RecordsState::quit, this));
*/

  //Attaching buttons
  sheet->addChild(marcoRecords);
  //sheet->addChild (quitButton);
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
    
}


bool RecordsState::volver(const CEGUI::EventArgs &e)
{
    _volver = true;
    
    return true;
}

void RecordsState::exit ()
{
    //Comento esto por que de momento voy a delegar la destrucción del GUI actual 
    //en el estado que tome el control después de salir de este.
    
    //_rendererCEGUI->destroySystem();
}

void RecordsState::pause ()
{
    
}

void RecordsState::resume ()
{
    
}

void RecordsState::keyPressed (const OIS::KeyEvent &e)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(static_cast<CEGUI::Key::Scan>(e.key));
  CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);

  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) {
    popState();
  }
    
}

void RecordsState::keyReleased (const OIS::KeyEvent &e)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(static_cast<CEGUI::Key::Scan>(e.key));
  CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);
    
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
    
}

void RecordsState::mouseMoved (const OIS::MouseEvent &e)
{
  CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setPosition(CEGUI::Vector2f(e.state.X.abs,e.state.Y.abs));
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(mousePos.d_x/float(e.state.width), mousePos.d_y/float(e.state.height));
}

void RecordsState::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertirBotonMouse(id));
}

void RecordsState::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertirBotonMouse(id));
}

bool RecordsState::frameStarted (const Ogre::FrameEvent& evt)
{
        
    return true;
}

bool RecordsState::frameEnded (const Ogre::FrameEvent& evt)
{
    if (_volver)
    {
        delete _rec;
        
        popState();
    }
        
    return true;
}


// Heredados de Ogre::Singleton.
RecordsState* RecordsState::getSingletonPtr ()
{
    return msSingleton;
}

RecordsState& RecordsState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

CEGUI::MouseButton RecordsState::convertirBotonMouse(OIS::MouseButtonID id)
{
  switch(id)
  {
    case OIS::MB_Right:		cout<< "boton derecho " << endl; return CEGUI::RightButton;
    case OIS::MB_Middle:	cout<< "boton central " << endl; return CEGUI::MiddleButton;
    case OIS::MB_Left:
    default: 			    cout<< "boton izquierdo" << endl; return CEGUI::LeftButton;
  }
}
