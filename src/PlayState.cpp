#include "PlayState.h"
#include "PauseState.h"
#include "IntroState.h"




//http://www.cplusplus.com/doc/tutorial/templates/          <--------Visita esta página para entender la linea justo debajo de esta
template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void PlayState::enter ()
{
    
    createPlayers();

/*
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));

  _exitGame = false;
*/

  _root = Ogre::Root::getSingletonPtr();
  _sceneMgr = _root->getSceneManager("SceneManager");
  _sceneMgr->setAmbientLight (Ogre::ColourValue (1, 1, 1));

  _camera = _sceneMgr->getCamera ("IntroCamera");
  //_viewport = _root->getAutoCreatedWindow()->getViewport(0);
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  double width = _viewport->getActualWidth ();
  double height = _viewport->getActualHeight ();
  _camera->setAspectRatio (width / height);
  _viewport->setBackgroundColour (ColourValue (0.0, 0.0, 0.0));

  //Tiempo entre frame y frame
  _deltaT = 0;

   
  //Creamos la escena
  createScene ();
  //Gui
  createGUI ();
  createGUIDefensaHumano();
  
  _camera->setPosition (Vector3 (0,12,18));
  _camera->lookAt (_sceneMgr->getRootSceneNode()->getPosition());
  _camera->setNearClipDistance (0.1);
  _camera->setFarClipDistance (100);
  
  _raySceneQuery = _sceneMgr->createRayQuery(Ogre::Ray());
  _selectedNode = NULL;
  _animState = NULL;
  _nodoCursor = _sceneMgr->createSceneNode("nodoCursor");
  _sceneMgr->getRootSceneNode()->addChild(_nodoCursor);
  _flip = false;
  _colocandoBarco = false;
  _exitGame = false;
  _tSpeed = 10.0;                              //Distancia en unidades del mundo virtual que queremos recorrer en un segundo cuando movamos cositas
  _estado = inicio;
  _turnoEnCurso = true;
  _idJugador = 0;
  _wait = false;
  _enRacha = 1;
  _ganador = NULL;
  _volverMenu = false;
  
  //activaGUIGameOver();
}

void PlayState::createGUIDefensaHumano()
{

  if (_jugadores[0]->_num_portaviones < _jugadores[0]->NUM_MAX_PORTAVIONES)
  {          
      CEGUI::Window* btnPortaviones = CEGUI::WindowManager::getSingleton ().createWindow ("TaharezLook/Button", "HLF/Portaviones");
      btnPortaviones->setText ("Portaviones");
      btnPortaviones->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.05, 0)));
      btnPortaviones->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.01, 0), CEGUI::UDim (0.01, 0)));
      btnPortaviones->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&PlayState::colocaPortaviones, this));
      CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(btnPortaviones);

  }
  
  if (_jugadores[0]->_num_acorazados < _jugadores[0]->NUM_MAX_ACORAZADOS)
  {
      CEGUI::Window* btnAcorazado = CEGUI::WindowManager::getSingleton ().createWindow ("TaharezLook/Button", "HLF/Acorazado");
      btnAcorazado->setText ("Acorazado");
      btnAcorazado->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.05, 0)));
      btnAcorazado->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.20, 0), CEGUI::UDim (0.01, 0)));
      btnAcorazado->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&PlayState::colocaAcorazado, this));
      CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(btnAcorazado);
  }

  if (_jugadores[0]->_num_lanchas < _jugadores[0]->NUM_MAX_LANCHAS)
  {
      CEGUI::Window* btnLancha = CEGUI::WindowManager::getSingleton ().createWindow ("TaharezLook/Button", "HLF/Lancha");
      btnLancha->setText ("Lancha");
      btnLancha->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.05, 0)));
      btnLancha->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.39, 0), CEGUI::UDim (0.01, 0)));
      btnLancha->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&PlayState::colocaLancha, this));
      CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(btnLancha);
  }
  
  if ( _jugadores[0]->_num_acorazados + _jugadores[0]->_num_lanchas + _jugadores[0]->_num_portaviones  == 
       _jugadores[0]->NUM_MAX_ACORAZADOS + _jugadores[0]->NUM_MAX_LANCHAS + _jugadores[0]->NUM_MAX_PORTAVIONES)
  {
       addSceneAtaque();
       createGUINext(); 
       createGUIInfo();
       _turnoEnCurso = true;
       _estado = jugando;
  }
  
  
  
}

void PlayState::createGUIAtaqueHumano()
{
    
    
}

void PlayState::addSceneAtaque()
{
  //Cogemos el tablero y le damos la vuelta    
  SceneNode *nodeTableroCol = _sceneMgr->getSceneNode("tableroCol");
  nodeTableroCol->roll(Ogre::Degree(180));
  
  //Casillas normales y casillas invisibles para hacer las consultas por RayQuery
  stringstream sauxnode;
  string s = "CasColATAQ_";
  string x = "CasATAQ_";
  
  Ogre::Real offsetX = 0.0;
  Ogre::Real offsetY = 0.0;
  Ogre::Real origen = 7.2;
  for (int i = 0; i < 10; i++)
  {
    offsetY = i * 0.6;
    for (int j = 0; j < 10; j++)
    {
        offsetX = j * 0.6;
                
        sauxnode << s << j << "_" << i;
        SceneNode *nodeCasillaCol = _sceneMgr->createSceneNode (sauxnode.str ());
        Entity *entCasillaCol = _sceneMgr->createEntity (sauxnode.str (), "CasillaCol.mesh");
        entCasillaCol->setQueryFlags(STAGE);
        nodeCasillaCol->attachObject(entCasillaCol);
        nodeCasillaCol->setVisible(false);
        nodeTableroCol->addChild (nodeCasillaCol);
        cout << "nodo " << sauxnode.str() << " creado. \n";
        
        sauxnode.str("");
        sauxnode << x << j << "_" << i;
        SceneNode *nodeCasilla = _sceneMgr->createSceneNode(sauxnode.str());
        Entity *entCasilla = _sceneMgr->createEntity(sauxnode.str(), "Casilla.mesh");
        entCasilla->setQueryFlags(CASILLA);
        nodeCasilla->attachObject(entCasilla);
        nodeTableroCol->addChild (nodeCasilla);
        cout << "nodo " << sauxnode.str() << " creado. \n";
        
        nodeCasilla->setPosition(origen - j  - offsetX , nodeCasilla->getPosition().y-1, (origen - i - offsetY) * -1);
        nodeCasillaCol->setPosition(origen - j - offsetX, nodeCasillaCol->getPosition().y-1, (origen - i - offsetY) * -1);
        
        sauxnode.str("");
    }
  }

}


void PlayState::createPlayers()
{

    Player *humano = new Humano();
    Player *cpu = new Cpu();
    
    cpu->AddBarco(portaviones);
    cpu->AddBarco(portaviones);
    cpu->AddBarco(acorazado);
    cpu->AddBarco(acorazado);
    cpu->AddBarco(acorazado);
    cpu->AddBarco(lancha);
    cpu->AddBarco(lancha);
    cpu->AddBarco(lancha);
    cpu->AddBarco(lancha);
    cpu->AddBarco(lancha);
    
    cpu->colocaBarcos();

    cpu->coutTableroDefensa();
    cpu->_resultado = nulo;
    
    _jugadores.push_back(humano);
    _jugadores.push_back(cpu);
    
}


void PlayState::createGUI ()
{
  _renderer = &CEGUI::OgreRenderer::bootstrapSystem();
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

  //Quit button
  CEGUI::Window* quitButton = CEGUI::WindowManager::getSingleton ().createWindow ("TaharezLook/Button", "HLF/QuitButton");
  quitButton->setText ("Quit");
  quitButton->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.05, 0)));
  quitButton->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.84, 0), CEGUI::UDim (0.01, 0)));
  quitButton->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&PlayState::quit, this));

  //Attaching buttons
  sheet->addChild (quitButton);
  CEGUI::System::getSingleton().getDefaultGUIContext ().setRootWindow (sheet);
}

void PlayState::createScene ()
{
  //Objeto movable "suelo" para consultar al sceneManager
  SceneNode *nodeTableroCol = _sceneMgr->createSceneNode("tableroCol");
  Entity *entTableroCol = _sceneMgr->createEntity("entTableroCol", "tableroCol.mesh");
  entTableroCol->setQueryFlags(STAGE);
  nodeTableroCol->attachObject(entTableroCol);
  nodeTableroCol->setVisible(true);
  _sceneMgr->getRootSceneNode()->addChild(nodeTableroCol);

  //Casillas normales y casillas invisibles para hacer las consultas por RayQuery
  stringstream sauxnode;
  string s = "CasillaCol_";
  string x = "Casilla_";
  
  Ogre::Real offsetX = 0.0;
  Ogre::Real offsetY = 0.0;
  Ogre::Real origen = -7.2;
  for (int i = 0; i < 10; i++)
  {
    offsetY = i * 0.6;
    for (int j = 0; j < 10; j++)
    {
        offsetX = j * 0.6;
                
        sauxnode << s << j << "_" << i;
        SceneNode *nodeCasillaCol = _sceneMgr->createSceneNode (sauxnode.str ());
        Entity *entCasillaCol = _sceneMgr->createEntity (sauxnode.str (), "CasillaCol.mesh");
        entCasillaCol->setQueryFlags(STAGE);
        nodeCasillaCol->attachObject(entCasillaCol);
        nodeCasillaCol->setVisible(false);
        nodeTableroCol->addChild (nodeCasillaCol);
        //cout << "nodo " << sauxnode.str() << " creado. \n";
        
        sauxnode.str("");
        sauxnode << x << j << "_" << i;
        SceneNode *nodeCasilla = _sceneMgr->createSceneNode(sauxnode.str());
        Entity *entCasilla = _sceneMgr->createEntity(sauxnode.str(), "Casilla.mesh");
        entCasilla->setQueryFlags(CASILLA);
        nodeCasilla->attachObject(entCasilla);
        nodeTableroCol->addChild (nodeCasilla);
        //cout << "nodo " << sauxnode.str() << " creado. \n";
        
        nodeCasilla->setPosition(origen + j  + offsetX , nodeCasilla->getPosition().y+1, origen + i + offsetY);
        nodeCasillaCol->setPosition(origen + j + offsetX, nodeCasillaCol->getPosition().y+1, origen + i + offsetY);
        
        sauxnode.str("");
    }
  }

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

bool PlayState::colocaPortaviones(const CEGUI::EventArgs &e)
{
    if (!_colocandoBarco)
    {
        Entity *entBarco;
        try
        {
           _nodoCursor->detachAllObjects();
           entBarco = _sceneMgr->createEntity("entPortaviones","Portaviones.mesh");
        } catch (Ogre::Exception &e)
        {
            cout << "\nFallo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n Nada, nada, pa'lante :D ";
        }

        entBarco = _sceneMgr->getEntity("entPortaviones");
        entBarco->setQueryFlags(BARCO);
        _nodoCursor->attachObject(entBarco);
        //_barcoSeleccionado = new Barco(portaviones);
        //_barcoSeleccionado->setOrientacion(horizontal);
        _barcoSeleccionado = Barco(portaviones);
        _barcoSeleccionado.setOrientacion(horizontal);
        _nodoCursor->setVisible(true);
        _colocandoBarco = true;
    }

    return true;
}

bool PlayState::colocaAcorazado(const CEGUI::EventArgs &e)
{
    if (!_colocandoBarco)
    {
        Entity *entBarco;
        try
        {
           _nodoCursor->detachAllObjects();
           entBarco = _sceneMgr->createEntity("entAcorazado","Acorazado.mesh");

        } catch (Ogre::Exception &e)
        {
            cout << "\nFallo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        }
        
        entBarco = _sceneMgr->getEntity("entAcorazado");
        entBarco->setQueryFlags(BARCO);
        _nodoCursor->attachObject(entBarco);
        //_barcoSeleccionado = new Barco(acorazado);
        //_barcoSeleccionado->setOrientacion(horizontal);
        _barcoSeleccionado = Barco(acorazado);
        _barcoSeleccionado.setOrientacion(horizontal);

        _nodoCursor->setVisible(true);
        _colocandoBarco = true;
    }

    return true;
}

bool PlayState::colocaLancha(const CEGUI::EventArgs &e)
{
    if (!_colocandoBarco)
    {
        Entity *entBarco;

        try
        {
           _nodoCursor->detachAllObjects();
           entBarco = _sceneMgr->createEntity("entLancha","Lancha.mesh");
        } catch (Ogre::Exception &e)
        {
            cout << "\nFallo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        }

        entBarco = _sceneMgr->getEntity("entLancha");
        entBarco->setQueryFlags(BARCO);
        _nodoCursor->attachObject(entBarco);
        //_barcoSeleccionado = new Barco(lancha);
        //_barcoSeleccionado->setOrientacion(horizontal);

        _barcoSeleccionado =  Barco(lancha);
        _barcoSeleccionado.setOrientacion(horizontal);
        _nodoCursor->setVisible(true);
        _colocandoBarco = true;
    }
    return true;
}


//Quit del GUI
bool PlayState::quit(const CEGUI::EventArgs &e)
{
  _exitGame = true;
  return true;
}


void PlayState::exit ()
{
  _sceneMgr->destroyQuery(_raySceneQuery);
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void PlayState::pause()
{
}

void PlayState::resume()
{
  // Se restaura el background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
}

bool PlayState::frameStarted(const Ogre::FrameEvent& evt)
{
  _deltaT = evt.timeSinceLastFrame; //Tiempo transcurrido desde que se pinto el último frame
  CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(_deltaT);

  _camera->moveRelative(_vtCamara * _deltaT * _tSpeed);
  if (_camera->getPosition().length() < 10.0) 
    _camera->moveRelative(-_vtCamara * _deltaT * _tSpeed);
    
  
  if (_estado == jugando)
  {
      actualizaGUIInfo();
      if (!_wait)
      {
          if (!hayGanador())
          {
              if (!_turnoEnCurso)
              {
                  _idJugador++;
                  _sceneMgr->getSceneNode("tableroCol")->roll(Ogre::Degree(180));
                  _turnoEnCurso = true;
              }

              _idJugador = _idJugador % _jugadores.size();
                  
              
              //cout << "\n Tipo Jugador " << (_jugadores[_idJugador]->getTipoJugador()==cpu?"cpu":"humano");
              
              if (_jugadores[_idJugador]->getTipoJugador() == cpu)
                  updateJuegoCpu();
           }
      }
      else
      {
          if (hayGanador())
          {
              cout << "HAY GANADOR" << endl;
              _estado = fin;
              activaGUIGameOver();
            
          }
      }
  }
  
  return true;
}

void PlayState::activaGUIGameOver()
{
      
  CEGUI::Window* marcoGameOver = CEGUI::WindowManager::getSingleton().createWindow ("TaharezLook/FrameWindow", "marcoGameOver");
  marcoGameOver->setText("GAME OVER");
  marcoGameOver->setSize (CEGUI::USize (CEGUI::UDim (0.50, 0), CEGUI::UDim (0.50, 0)));
  marcoGameOver->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.20,0), CEGUI::UDim (0.01, 0)));
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(marcoGameOver);

  _guardadorRecords = new SetRecords();
  _guardadorRecords->setRecordAIntroducir("temp",1100);
  
  if (_guardadorRecords->entra())
  {
      CEGUI::Window* lblPlayer = CEGUI::WindowManager::getSingleton().createWindow ("TaharezLook/Label", "lblPlayer");
      lblPlayer->setText ("NOMBRE:");
      lblPlayer->setSize (CEGUI::USize (CEGUI::UDim (0.30, 0), CEGUI::UDim (0.1, 0)));
      lblPlayer->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.1,0), CEGUI::UDim (0.11, 0)));
      CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("marcoGameOver")->addChild(lblPlayer);

      CEGUI::Window* txtNombre = CEGUI::WindowManager::getSingleton().createWindow ("TaharezLook/Editbox", "txtNombre");
      txtNombre->setText ("");
      txtNombre->setSize (CEGUI::USize (CEGUI::UDim (0.50, 0), CEGUI::UDim (0.1, 0)));
      txtNombre->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.33,0), CEGUI::UDim (0.11, 0)));
      CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("marcoGameOver")->addChild(txtNombre);
      
      CEGUI::Window* btnGuardarRecord = CEGUI::WindowManager::getSingleton().createWindow ("TaharezLook/Button", "btnGuardarRecord");
      btnGuardarRecord->setText("Guardar Record");
      btnGuardarRecord->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.10, 0)));
      btnGuardarRecord->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.70,0), CEGUI::UDim (0.43, 0)));
      btnGuardarRecord->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&PlayState::guardaRecord, this));
      CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("marcoGameOver")->addChild(btnGuardarRecord);
      
  }

  CEGUI::Window* lblPuntuacion = CEGUI::WindowManager::getSingleton().createWindow ("TaharezLook/Label", "lblPuntuacion");
  stringstream s;
  s << "PUNTUACION: " <<  _jugadores[_idJugador]->_puntuacion;
  lblPuntuacion->setText (s.str());
  lblPuntuacion->setSize (CEGUI::USize (CEGUI::UDim (0.30, 0), CEGUI::UDim (0.1, 0)));
  lblPuntuacion->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.1,0), CEGUI::UDim (0.30, 0)));
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("marcoGameOver")->addChild(lblPuntuacion);
  
  CEGUI::Window* btnMenuPpal = CEGUI::WindowManager::getSingleton().createWindow ("TaharezLook/Button", "btnMenuPpal");
  btnMenuPpal->setText("Menu ppal.");
  btnMenuPpal->setSize (CEGUI::USize (CEGUI::UDim (0.20, 0), CEGUI::UDim (0.10, 0)));
  btnMenuPpal->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.20,0), CEGUI::UDim (0.43, 0)));
  btnMenuPpal->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&PlayState::menu, this));
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("marcoGameOver")->addChild(btnMenuPpal);
    
/*    
  _guardadorRecords = new SetRecords();
  _guardadorRecords->setRecordAIntroducir("maquina",1100);
  if (_guardadorRecords->entra())
      _guardadorRecords->guardaRecord();
    
    if (_guardadorRecords) delete _guardadorRecords;
*/
  
    
}

bool PlayState::guardaRecord(const CEGUI::EventArgs &e)
{
    if (_guardadorRecords)
    {
        string nombre = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("marcoGameOver")->getChild("txtNombre")->getText().c_str();
        _guardadorRecords->setRecordAIntroducir(nombre,_jugadores[_idJugador]->_puntuacion);
        _guardadorRecords->guardaRecord();
         delete _guardadorRecords;
    }

    return true;
}

bool PlayState::menu(const CEGUI::EventArgs &e)
{
    _volverMenu = true;
    
    return true;
}


bool PlayState::hayGanador()
{
    bool res = false;
    for (size_t i = 0; i<_jugadores.size(); ++i)
        if ((res = _jugadores[i]->hePerdido())) break;
    
    //cout << "Ganador? " << res << endl;
    return res;
}

void PlayState::updateJuegoCpu()
{
    _turnoEnCurso = true;
        
    do
    {
        _jugadores[_idJugador]->coutTableroAtaque();
        _jugadores[_idJugador]->coutTableroDefensa();
        cout << "la cpu va a mover \n";
        _jugadores[_idJugador]->mueve(); // Player cpu actualizará la variable miembro _casillaTiro, luego preguntamos que ha pasado.
        cout << "la cpu a elegido moverse a " << _jugadores[_idJugador]->_casillaTiro  << endl;
        ponMisilacoTableroDefensa(_jugadores[_idJugador]->getTipoJugador());
        _jugadores[_idJugador]->_resultado = _jugadores[(_idJugador+1)%2]->resultadoTiroOponente(_jugadores[_idJugador]->_casillaTiro);
        _enRacha++;
        puntua();
    }while(_jugadores[_idJugador]->_resultado != errado);
    
    _enRacha = 1;
    _wait = true;
    _turnoEnCurso = false;
}

void PlayState::puntua()
{
    switch(_jugadores[_idJugador]->_resultado)
    {
        case tocado:        _jugadores[_idJugador]->_puntuacion += PUNTOS_TOCADO * _enRacha; break;
        case tocadoHundido: _jugadores[_idJugador]->_puntuacion += PUNTOS_HUNDIDO * _enRacha; break;
        default:;
    }
}


void PlayState::createGUINext()
{

  CEGUI::Window* btnNext = CEGUI::WindowManager::getSingleton ().createWindow ("TaharezLook/Button", "HLF/NextButton");
  btnNext->setText ("NEXT");
  btnNext->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.05, 0)));
  btnNext->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.01,0), CEGUI::UDim (0.01, 0)));
  btnNext->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&PlayState::siguiente, this));
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(btnNext);
}

void PlayState::createGUIInfo()
{
  CEGUI::Window* btnNext = CEGUI::WindowManager::getSingleton ().createWindow ("TaharezLook/Button", "HLF/NextButton");
  btnNext->setText ("NEXT");
  btnNext->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.05, 0)));
  btnNext->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.01,0), CEGUI::UDim (0.01, 0)));
  btnNext->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&PlayState::siguiente, this));
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(btnNext);
    
  CEGUI::Window* txtPuntos = CEGUI::WindowManager::getSingleton().createWindow ("TaharezLook/StaticText", "txtPuntos");
  std::stringstream s;
  s << "Puntos: " << _jugadores[_idJugador]->_puntuacion;
  txtPuntos->setText (s.str());
  txtPuntos->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.05, 0)));
  txtPuntos->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.20,0), CEGUI::UDim (0.01, 0)));
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(txtPuntos);

  CEGUI::Window* txtMov = CEGUI::WindowManager::getSingleton().createWindow ("TaharezLook/StaticText", "txtMov");
  s.str("");;
  s << "Movimiento: [" << _jugadores[_idJugador]->_casillaTiro.x << "," << _jugadores[_idJugador]->_casillaTiro.y << "]";
  txtMov->setText (s.str());
  txtMov->setSize (CEGUI::USize (CEGUI::UDim (0.30, 0), CEGUI::UDim (0.05, 0)));
  txtMov->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.39,0), CEGUI::UDim (0.01, 0)));
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(txtMov);
}

void PlayState::actualizaGUIInfo()
{
    std::stringstream s;
    s << "Puntos: " << _jugadores[_idJugador]->_puntuacion;
    CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("txtPuntos")->setText(s.str());
    s.str("");
    s << "Movimiento: [" << _jugadores[_idJugador]->_casillaTiro.x << "," << _jugadores[_idJugador]->_casillaTiro.y << "]";
    CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("txtMov")->setText(s.str());
}

bool PlayState::siguiente(const CEGUI::EventArgs &e)
{
    _wait = false;
    
    return true;
}

void PlayState::ponMisilacoTableroDefensa(tipoPlayer tipo)
{
    stringstream s,m,t,aux;
    s << "Casilla_" << _jugadores[_idJugador]->_casillaTiro.x << "_" << _jugadores[_idJugador]->_casillaTiro.y;
    switch (tipo)
    {
        case cpu:    aux << "cpu_Mis_"; break;
        case humano: aux << "humaMis_"; break;
        default:;
    }
    t << aux.str() << _jugadores[_idJugador]->_casillaTiro.x << "_" << _jugadores[_idJugador]->_casillaTiro.y;
    SceneNode *nodoAux = _sceneMgr->createSceneNode(t.str());
    m << aux.str() << _jugadores[_idJugador]->_casillaTiro.x << "_" << _jugadores[_idJugador]->_casillaTiro.y;
    Entity *entMisilaco = _sceneMgr->createEntity(m.str(),"misil.mesh");
    entMisilaco->setQueryFlags(MISIL);
    nodoAux->attachObject(entMisilaco);
    _sceneMgr->getSceneNode(s.str())->addChild(nodoAux);
    nodoAux->setPosition(0,0.2,0);
    nodoAux->rotate(_nodoCursor->getOrientation());
    nodoAux->roll(Ogre::Degree(15));
}

void PlayState::ponMisilacoTableroAtaque()
{
    stringstream s,m,t;
    s << "CasATAQ_" << _jugadores[_idJugador]->_casillaTiro.x << "_" << _jugadores[_idJugador]->_casillaTiro.y;
    t << "humaMis_" << _jugadores[_idJugador]->_casillaTiro.x << "_" << _jugadores[_idJugador]->_casillaTiro.y;
    SceneNode *nodoAux = _sceneMgr->createSceneNode(t.str());
    m << "humaMis_" << _jugadores[_idJugador]->_casillaTiro.x << "_" << _jugadores[_idJugador]->_casillaTiro.y;
    Entity *entMisilaco = _sceneMgr->createEntity(m.str(),"misil.mesh");
    entMisilaco->setQueryFlags(MISIL);
    nodoAux->attachObject(entMisilaco);
    _sceneMgr->getSceneNode(s.str())->addChild(nodoAux);
    nodoAux->setPosition(0,-0.2,0);
    nodoAux->rotate(_nodoCursor->getOrientation());
    nodoAux->roll(Ogre::Degree(15));
    nodoAux->pitch(Ogre::Degree(180));
    
}

bool PlayState::frameEnded(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
    
  if (_volverMenu)
      changeState(IntroState::getSingletonPtr());
  
  return true;
}

void PlayState::keyPressed(const OIS::KeyEvent &e)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(static_cast<CEGUI::Key::Scan>(e.key));
  CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);
  
  _vtBarco = Ogre::Vector3(0,0,0);
  _vtCamara = Ogre::Vector3(0,0,0);

  switch (e.key)
  {
    case OIS::KC_UP:		_vtCamara+=Ogre::Vector3(0,0,-1); cout<< "UP" << endl; break;
    case OIS::KC_DOWN:		_vtCamara+=Ogre::Vector3(0,0,1);  cout<< "DOWN" << endl; break;
    //case OIS::KC_LEFT:		_vtCamara+=Ogre::Vector3(-1,0,0); cout<< "LEFT" << endl; break;
    //case OIS::KC_RIGHT:		_vtCamara+=Ogre::Vector3(1,0,0);  cout<< "RIGHT" << endl; break;
    case OIS::KC_A:	_vtBarco+=Vector3(1,0,0);  break;
    case OIS::KC_S:	_vtBarco+=Vector3(0,0,-1); break;
    case OIS::KC_D:	_vtBarco+=Vector3(-1,0,0); break;
    case OIS::KC_W:	_vtBarco+=Vector3(0,0,1);  break;
    default:   	cout << "VTBarco " << _vtBarco << endl;
                cout << "VTCamara " << _vtCamara << endl;
  }
  

  // Tecla p --> PauseState.
  if (e.key == OIS::KC_P) {
    pushState(PauseState::getSingletonPtr());
  }
}

void PlayState::keyReleased(const OIS::KeyEvent &e)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(static_cast<CEGUI::Key::Scan>(e.key));
  CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);
  
  _vtBarco = Ogre::Vector3(0,0,0);
  _vtCamara = Ogre::Vector3(0,0,0);
  
  if (e.key == OIS::KC_ESCAPE)
     _exitGame = true;
}

void PlayState::mouseMoved(const OIS::MouseEvent &e)
{
    //CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(e.state.X.rel, e.state.Y.rel);  
    //CEGUI::System::getSingleton().ge tDefaultGUIContext().injectMouseMove(e.state.X.abs,e.state.Y.abs);

    CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
    /*  
    cout << "PRE CEGUI: " << mousePos.d_x << "," << mousePos.d_y << endl;
    cout << "OIS ABS: " << e.state.X.abs << "," << e.state.Y.abs << "     OIS REL: " << e.state.X.rel << "," << e.state.Y.rel << endl;
    cout << "OIS State Width: " << e.state.width << "     State Height: " << e.state.height << endl;
    */
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setPosition(CEGUI::Vector2f(e.state.X.abs,e.state.Y.abs));
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(mousePos.d_x/float(e.state.width), mousePos.d_y/float(e.state.height));
    mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
    //cout << "POST CEGUI: " << mousePos.d_x << "," << mousePos.d_y << endl;

    int posx = e.state.X.abs;
    int posy = e.state.Y.abs;
    Ray r = setRayQuery(posx, posy, CASILLA);
    (void)r;
    RaySceneQueryResult &result = _raySceneQuery->execute();
    RaySceneQueryResult::iterator it;
    it = result.begin();
    if (it != result.end()) 
    {
      if (_selectedNode != it->movable->getParentSceneNode())
      {
          if (_selectedNode) _selectedNode->showBoundingBox(false);
          _selectedNode = it->movable->getParentSceneNode();
          _selectedNode->showBoundingBox(true);
      }
    }
      
    //if (_selectedNode) cout << _selectedNode->getName() << " POSICION " << _selectedNode->getPosition()<< endl; //<< "," << _selectedNode->getPosition().y <<
                                                    //"," << _selectedNode->getPosition().z << endl;
    if (_colocandoBarco)
    {
      if (_selectedNode)
        _sceneMgr->getSceneNode("nodoCursor")->setPosition(_selectedNode->getPosition().x,_selectedNode->getPosition().y + 0.1, _selectedNode->getPosition().z);
    }
    else
      if (_nodoCursor)
         _sceneMgr->getSceneNode("nodoCursor")->setVisible(false);

}

void PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    //mbLeft = _mouse->getMouseState().buttonDown(OIS::MB_Left);
    //mbMiddle = _mouse->getMouseState().buttonDown(OIS::MB_Middle);
    //mbRight =  _mouse->getMouseState().buttonDown(OIS::MB_Right);

    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertirBotonMouse(id));

    switch (_estado)
    {
        case inicio:
                    if (id == OIS::MB_Left)
                    {
                       if (_colocandoBarco)
                           if (_selectedNode)
                               if (static_cast<Humano*>(_jugadores[0])->colocaBarcos(extraePosicionCasilla(_selectedNode->getName()),_barcoSeleccionado))
                               {
                                  //addBarcoAlTablero(_jugadores[0],*_barcoSeleccionado);
                                  addBarcoAlTablero(_jugadores[0],_barcoSeleccionado);
                                  _colocandoBarco = false;
                                  _jugadores[0]->coutTableroDefensa();      
                                  //actualizaGUIColocacion(_barcoSeleccionado->getTipo());
                                  actualizaGUIColocacion(_barcoSeleccionado.getTipo());
                               }
                    }
                    if (id == OIS::MB_Right)
                    {
                       if (_colocandoBarco)
                       { 
                           _nodoCursor->yaw(Ogre::Degree(90));
                           //switch (_barcoSeleccionado->getOrientacion())
                           switch (_barcoSeleccionado.getOrientacion())
                           {
                               //case horizontal: _barcoSeleccionado->setOrientacion(vertical); break;
                               //case vertical: _barcoSeleccionado->setOrientacion(horizontal);
                               case horizontal: _barcoSeleccionado.setOrientacion(vertical); break;
                               case vertical: _barcoSeleccionado.setOrientacion(horizontal);

                           }
                       }
                    }
                    break;
        
        case jugando:
                    if (!hayGanador())
                    {
                        if (_jugadores[_idJugador]->getTipoJugador() == humano)
                        {
                           _jugadores[_idJugador]->_casillaTiro = extraePosicionCasilla(_selectedNode->getName());
                           if (_jugadores[_idJugador]->_casilleroAtaque[_jugadores[_idJugador]->_casillaTiro.x][_jugadores[_idJugador]->_casillaTiro.y]._estado == neutro)
                           { 
                               _jugadores[_idJugador]->_resultado = _jugadores[(_idJugador+1)%2]->resultadoTiroOponente(_jugadores[_idJugador]->_casillaTiro);
                               ponMisilacoTableroAtaque();
                               switch (_jugadores[_idJugador]->_resultado) //res
                               {
                                   case errado: static_cast<Entity *>(_selectedNode->getAttachedObject(0))->getSubEntity(0)->setMaterialName("MaterialAgua");
                                                _enRacha = 1;
                                                _turnoEnCurso = false;
                                                break;
                                   case tocado: 
                                   case tocadoHundido:static_cast<Entity *>(_selectedNode->getAttachedObject(0))->getSubEntity(0)->setMaterialName("MaterialTocado"); 
                                                      _enRacha++;
                                                      if (hayGanador()) 
                                                          //_estado = fin;
                                                          _wait = true;
                                                      break;
                                   default:;
                               }
                               puntua();
                               _jugadores[_idJugador]->actualizaTableroAtaque(_jugadores[_idJugador]->_resultado);
                               _jugadores[_idJugador]->coutTableroAtaque();
                           }    
                        }
                    }
                    else _wait = true;
                    break;
        case animando: break;
        case fin:  break;
    }

}

void PlayState::addBarcoAlTablero(Player *jugador, Barco barco)
{
    stringstream nombreNodo;
    stringstream nombreEnt;
    stringstream mesh;
    string n = "nodo";
    string e = "ent";
    switch (barco.getTipo())
    {
        case lancha:         nombreNodo << n << "Lancha_" << jugador->_num_lanchas;
                             nombreEnt << e << "Lancha_" << jugador->_num_lanchas; 
                             mesh << "Lancha.mesh"; break;
        case acorazado:      nombreNodo << n << "Acorazado_" << jugador->_num_acorazados;
                             nombreEnt << e << "Acorazado_" << jugador->_num_acorazados;
                             mesh << "Acorazado.mesh"; break;
        case portaviones:    nombreNodo << n << "Portaviones_" << jugador->_num_portaviones;
                             nombreEnt << e << "Portaviones_" << jugador->_num_portaviones; 
                             mesh << "Portaviones.mesh"; break; 
        default: ;
    }
    
    SceneNode *nodoAux = _sceneMgr->createSceneNode(nombreNodo.str());
    Entity *entAux = _sceneMgr->createEntity(nombreEnt.str(), mesh.str());
    entAux->setQueryFlags(BARCO);
    nodoAux->attachObject(entAux);
    _sceneMgr->getSceneNode(_selectedNode->getName())->addChild(nodoAux);
    nodoAux->setPosition(0,0.1,0);
    nodoAux->rotate(_nodoCursor->getOrientation());
    if (_nodoCursor)
    {
        _nodoCursor->setVisible(false);
        if (Ogre::Math::Cos(_nodoCursor->getOrientation().getYaw()) != 0)
            // Si el coseno del angulo sobre el eje Y no da 0 es que no estaba horizontal, lo reseteamos.
            _nodoCursor->yaw(-_nodoCursor->getOrientation().getYaw());
    }
    
    cout << "posicion barco " << nodoAux->getPosition().x << "," << nodoAux->getPosition().y << "," <<nodoAux->getPosition().z << endl;
    cout << "Padre barco en casilla " << _selectedNode->getName() << ", " << _selectedNode->getPosition() << endl;
}

void PlayState::actualizaGUIColocacion(tipoBarco tipo)
{
    switch (tipo)
    {
        case lancha:        _jugadores[0]->_num_lanchas++; break;
        case acorazado:     _jugadores[0]->_num_acorazados++; break;
        case portaviones:   _jugadores[0]->_num_portaviones++; break;
        default: ;
    }
    
    _renderer->destroySystem();
    createGUI();
    createGUIDefensaHumano();
}

Ogre::Vector2 PlayState::extraePosicionCasilla(string nombre)
{
       string xs,ys;
       string aux = _selectedNode->getName().substr(8); //Sacamos la parte que nos interesa
       xs = aux.substr(0,aux.find("_"));                //extraigo la x
       ys = aux.substr(aux.find("_")+1);                //extraigo la y
       int x = atoi(xs.c_str());                        // y las convierto a entero
       int y = atoi(ys.c_str());
       
       return Ogre::Vector2(x,y);

}

void PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertirBotonMouse(id));
  
   if (id == OIS::MB_Middle)
   {
     _rotCamarax = 0.0;
     _rotCamaray = 0.0;
   }

}

PlayState* PlayState::getSingletonPtr ()
{
    return msSingleton;
}

PlayState& PlayState::getSingleton ()
{ 
    assert(msSingleton);
    return *msSingleton;
}

CEGUI::MouseButton PlayState::convertirBotonMouse(OIS::MouseButtonID id)
{
    switch(id)
    {
        case OIS::MB_Right:		return CEGUI::RightButton;
        case OIS::MB_Middle:	return CEGUI::MiddleButton;
        case OIS::MB_Left:
        default: 			    return CEGUI::LeftButton;
    }
}

Ray PlayState::setRayQuery(int posx, int posy, uint32 mask) 
{
  Ray rayMouse = _camera->getCameraToViewportRay(posx/float(_root->getAutoCreatedWindow()->getWidth()), 
                                                 posy/float(_root->getAutoCreatedWindow()->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  _raySceneQuery->setQueryMask(mask);
  return (rayMouse);
}

