#include "PlayState.h"
#include "PauseState.h"




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

  /*RenderWindow *_renderWindow = _root->initialise(true,"Hundir la flota");
  //_renderer = _root->initialise(true,"Hundir la flota");
  //_sceneMgr = _root->createSceneManager (ST_INTERIOR,"SceneManager");*/
  _sceneMgr = _root->getSceneManager("SceneManager");
  _sceneMgr->setAmbientLight (Ogre::ColourValue (1, 1, 1));



  //_camera = _sceneMgr->createCamera ("IntroCamera");
  _camera = _sceneMgr->getCamera ("IntroCamera");

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  //_viewport = _renderWindow->addViewport (_camera);
  double width = _viewport->getActualWidth ();
  double height = _viewport->getActualHeight ();
  _camera->setAspectRatio (width / height);
  _viewport->setBackgroundColour (ColourValue (0.0, 0.0, 0.0));

  //Tiempo entre frame y frame
  _deltaT = 0;

  _mecer = -10;

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
  _nodoCursor = _sceneMgr->createSceneNode("nodoCursor");
  _sceneMgr->getRootSceneNode()->addChild(_nodoCursor);
  _flip = false;
  _colocandoBarco = false;
  _exitGame = false;
  _tSpeed = 10.0;                              //Distancia en unidades del mundo virtual que queremos recorrer en un segundo cuando movamos cositas
  

}

void PlayState::createGUIDefensaHumano()
{

  if (pm._jugadores[0]->num_portaviones < pm._jugadores[0]->NUM_MAX_PORTAVIONES)
  {          
      CEGUI::Window* btnPortaviones = CEGUI::WindowManager::getSingleton ().createWindow ("TaharezLook/Button", "HLF/Portaviones");
      btnPortaviones->setText ("Portaviones");
      btnPortaviones->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.05, 0)));
      btnPortaviones->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.01, 0), CEGUI::UDim (0.01, 0)));
      btnPortaviones->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&PlayState::colocaPortaviones, this));
      CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(btnPortaviones);

  }
  
  if (pm._jugadores[0]->num_acorazados < pm._jugadores[0]->NUM_MAX_ACORAZADOS)
  {
      CEGUI::Window* btnAcorazado = CEGUI::WindowManager::getSingleton ().createWindow ("TaharezLook/Button", "HLF/Acorazado");
      btnAcorazado->setText ("Acorazado");
      btnAcorazado->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.05, 0)));
      btnAcorazado->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.20, 0), CEGUI::UDim (0.01, 0)));
      btnAcorazado->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&PlayState::colocaAcorazado, this));
      CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(btnAcorazado);
  }

  if (pm._jugadores[0]->num_lanchas < pm._jugadores[0]->NUM_MAX_LANCHAS)
  {
      CEGUI::Window* btnLancha = CEGUI::WindowManager::getSingleton ().createWindow ("TaharezLook/Button", "HLF/Lancha");
      btnLancha->setText ("Lancha");
      btnLancha->setSize (CEGUI::USize (CEGUI::UDim (0.15, 0), CEGUI::UDim (0.05, 0)));
      btnLancha->setPosition (CEGUI::UVector2 (CEGUI::UDim (0.39, 0), CEGUI::UDim (0.01, 0)));
      btnLancha->subscribeEvent (CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber (&PlayState::colocaLancha, this));
      CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(btnLancha);
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
    
    cpu->colocaBarcos();

    
    pm.addPlayer(humano);
    pm.addPlayer(cpu);
    pm.espera(true);

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
 /* 
  StaticGeometry *tablero = _sceneMgr->createStaticGeometry ("Tablero");
  Entity *entTablero = _sceneMgr->createEntity ("tablero.mesh");
  tablero->addEntity (entTablero, Vector3 (0, 0, 0)); // Añadir la entidad a tablero siempre antes de llamar a build()
  tablero->build ();
*/
  
  //Objeto movable "suelo" para consultar al sceneManager
  SceneNode *nodeTableroCol = _sceneMgr->createSceneNode("tableroCol");
  Entity *entTableroCol = _sceneMgr->createEntity("entTableroCol", "tableroCol.mesh");
  entTableroCol->setQueryFlags(STAGE);
  nodeTableroCol->attachObject(entTableroCol);
  nodeTableroCol->setVisible(true);
  _sceneMgr->getRootSceneNode()->addChild(nodeTableroCol);
  //nodeTableroCol->setPosition(nodeTableroCol->getPosition().x,-10,nodeTableroCol->getPosition().z);


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
        cout << "nodo " << sauxnode.str() << " creado. \n";
        
        sauxnode.str("");
        sauxnode << x << j << "_" << i;
        SceneNode *nodeCasilla = _sceneMgr->createSceneNode(sauxnode.str());
        Entity *entCasilla = _sceneMgr->createEntity(sauxnode.str(), "Casilla.mesh");
        entCasilla->setQueryFlags(CASILLA);
        nodeCasilla->attachObject(entCasilla);
        nodeTableroCol->addChild (nodeCasilla);
        cout << "nodo " << sauxnode.str() << " creado. \n";
        
        nodeCasilla->setPosition(origen + j  + offsetX , nodeCasilla->getPosition().y+1, origen + i + offsetY);
        nodeCasilla->setVisible(true);
        nodeCasillaCol->setPosition(origen + j + offsetX, nodeCasillaCol->getPosition().y+1, origen + i + offsetY);
        
        sauxnode.str("");
    }
  }
  
  // Comentario referente a la exportación de Blender a Ogre:
  // Hay que tener cuidado a la hora de exportar las figuras y tener claro
  // lo que se exporta. OgreToBlender exportará sólo la figura seleccionada
  // y sus materiales (texturas) asociados. Por lo que es recomendable renombrar
  // tanto los nombres de las figuras como de sus materiales asociados de lo
  // contrario sobreescribirá aquellos que se llamen igual. Lo cual puede estar
  // bien si pretendemos usar los mismos materiales para distintas figuras.
  //  SceneNode *nodoBarquito = _sceneMgr->createSceneNode ("Barquito");
  //  Entity *entBarquito = _sceneMgr->createEntity ("barquito", "barquito.mesh");
  //  entBarquito->setQueryFlags(BARCO);
  //  nodoBarquito->attachObject (entBarquito);
  //  _sceneMgr->getSceneNode("Casilla_1_1")->addChild(nodoBarquito);
  //  //_sceneMgr->getRootSceneNode ()->addChild (nodoBarquito);

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
        _barcoSeleccionado = new Barco(portaviones);
        _barcoSeleccionado->setOrientacion(horizontal);
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
        _barcoSeleccionado = new Barco(acorazado);
        _barcoSeleccionado->setOrientacion(horizontal);
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
        _barcoSeleccionado = new Barco(lancha);
        _barcoSeleccionado->setOrientacion(horizontal);
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

  //int fps = 1.0 / _deltaT;                              //Para calcular el rendimiento

  _camera->moveRelative(_vtCamara * _deltaT * _tSpeed);
  if (_camera->getPosition().length() < 10.0) 
    _camera->moveRelative(-_vtCamara * _deltaT * _tSpeed);
    

/*
  _mecer = _mecer % 10;
  if (!_mecer)
      _mecer *= -1;
  _sceneMgr->getSceneNode("Barquito")->roll(Ogre::Degree(_mecer * _deltaT));
  _mecer++;
*/  
  
  return true;
}

bool PlayState::frameEnded(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
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
  
  cout << _camera->getPosition() << endl;
  
  _r = 0;
  if(e.key == OIS::KC_R) _r+=180;

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
  _r = 0;
  
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

    /*
    _rotCamarax = e.state.X.rel * _deltaT * -1;
    _rotCamaray = e.state.Y.rel * _deltaT * -1;
    _camera->yaw(Radian(_rotCamarax));
    _camera->pitch(Radian(_rotCamaray));
    */   

    int posx = e.state.X.abs;
    int posy = e.state.Y.abs;
    Ray r = setRayQuery(posx, posy, CASILLA);
    (void)r;
    RaySceneQueryResult &result = _raySceneQuery->execute();
    RaySceneQueryResult::iterator it;
    it = result.begin();
    if (it != result.end()) 
    {
    /*
            if (it->movable->getParentSceneNode()->getName() == "Col_Suelo") 
            {
              SceneNode *nodeaux = _sceneManager->createSceneNode();
              int i = rand()%2;   std::stringstream saux;
              saux << "Cube" << i+1 << ".mesh";
              Entity *entaux = _sceneManager->createEntity(saux.str());
              entaux->setQueryFlags(i?CUBE1:CUBE2);
              nodeaux->attachObject(entaux);
              nodeaux->translate(r.getPoint(it->distance));
              _sceneManager->getRootSceneNode()->addChild(nodeaux);
            }
    */
      if (_selectedNode != it->movable->getParentSceneNode())
      {
          if (_selectedNode) _selectedNode->showBoundingBox(false);
          _selectedNode = it->movable->getParentSceneNode();
          _selectedNode->showBoundingBox(true);
      }
    }
      
    if (_selectedNode) cout << _selectedNode->getName() << endl; // << " POSICION_X " << _selectedNode->getPosition().x << endl; //<< "," << _selectedNode->getPosition().y <<
                                                    //"," << _selectedNode->getPosition().z << endl;
    if (_colocandoBarco)
    {
      if (_selectedNode)
        _sceneMgr->getSceneNode("nodoCursor")->setPosition(_selectedNode->getPosition());
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

    if (id == OIS::MB_Left)
    {
       if (_colocandoBarco)
       { 
           if (_selectedNode)
           { 
               if (static_cast<Humano*>(pm._jugadores[0])->colocaBarcos(extraePosicionCasilla(_selectedNode->getName()),*_barcoSeleccionado))
               {
                  addBarcoAlTablero(pm._jugadores[0],*_barcoSeleccionado);
                  _colocandoBarco = false;
                  
                  for (size_t y = 0; y < pm._jugadores[0]->_casilleroDefensa.size(); y ++)
                  {    
                        cout << "\n";
                        for (size_t x = 0; x < pm._jugadores[0]->_casilleroDefensa[y].size(); x ++)
                            cout << pm._jugadores[0]->_casilleroDefensa[x][y]._vacia << " ";
                  }
                  cout << "\n\n";
                        
                  actualizaGUIColocacion(_barcoSeleccionado->getTipo());
               }
           }
       }   
               
    }

/*
    if (id == OIS::MB_Middle)
    {
     cout << "se ha presionado el boton central" << endl;
     _rotCamarax = e.state.X.abs * _deltaT * -1;
     _rotCamaray = e.state.Y.abs * _deltaT * -1;
    }
*/

    if (id == OIS::MB_Right)
    {
       if (_colocandoBarco)
       { 
           _nodoCursor->yaw(Ogre::Degree(90));
           switch (_barcoSeleccionado->getOrientacion())
           {
               case horizontal: _barcoSeleccionado->setOrientacion(vertical); break;
               case vertical: _barcoSeleccionado->setOrientacion(horizontal);
           }
       }
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
        case lancha:         nombreNodo << n << "Lancha_" << jugador->num_lanchas;
                             nombreEnt << e << "Lancha_" << jugador->num_lanchas; 
                             mesh << "Lancha.mesh"; break;
        case acorazado:      nombreNodo << n << "Acorazado_" << jugador->num_acorazados;
                             nombreEnt << e << "Acorazado_" << jugador->num_acorazados;
                             mesh << "Acorazado.mesh"; break;
        case portaviones:    nombreNodo << n << "Portaviones_" << jugador->num_portaviones;
                             nombreEnt << e << "Portaviones_" << jugador->num_portaviones; 
                             mesh << "Portaviones.mesh"; break; 
        default: ;
    }
    
    SceneNode *nodoAux = _sceneMgr->createSceneNode(nombreNodo.str());
    Entity *entAux = _sceneMgr->createEntity(nombreEnt.str(), mesh.str());
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
        case lancha:        pm._jugadores[0]->num_lanchas++; break;
        case acorazado:     pm._jugadores[0]->num_acorazados++; break;
        case portaviones:   pm._jugadores[0]->num_portaviones++; break;
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
        case OIS::MB_Right:		cout<< "boton derecho " << endl; return CEGUI::RightButton;
        case OIS::MB_Middle:	cout<< "boton central " << endl; return CEGUI::MiddleButton;
        case OIS::MB_Left:
        default: 			    cout<< "boton izquierdo" << endl; return CEGUI::LeftButton;
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

