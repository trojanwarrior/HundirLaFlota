/*********************************************************************
 * Módulo 1. Curso de Experto en Desarrollo de Videojuegos
 * Autor: David Vallejo Fernández    David.Vallejo@uclm.es
 *
 * Código modificado a partir de Managing Game States with OGRE
 * http://www.ogre3d.org/tikiwiki/Managing+Game+States+with+OGRE
 * Inspirado en Managing Game States in C++
 * http://gamedevgeek.com/tutorials/managing-game-states-in-c/
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/

#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include "Player.h"
#include "Humano.h"
#include "Cpu.h"

#define STAGE   1 << 0 // mascara para el escenario
#define BARCO   1 << 1 // mascara para un barco, en realidad será una casilla donde haya un trozo de barco
#define CASILLA 1 << 2

enum estadoJuego
{
    inicio,
    jugando,
    animando,
    fin
};

class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
 public:
  PlayState () {}
  
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

  //Callbacks del GUI
  //Quit del GUI
  bool quit(const CEGUI::EventArgs &e);
  bool colocaPortaviones(const CEGUI::EventArgs &e);
  bool colocaAcorazado(const CEGUI::EventArgs &e);
  bool colocaLancha(const CEGUI::EventArgs &e);
  bool siguiente(const CEGUI::EventArgs &e);



  // Heredados de Ogre::Singleton.
  static PlayState& getSingleton ();
  static PlayState* getSingletonPtr ();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  CEGUI::OgreRenderer* _renderer;
  Ogre::Real _deltaT;
  Ogre::AnimationState *_animState;

  void createGUI();
  void createScene();
  void createPlayers();
  void createGUIDefensaHumano();
  void createGUIAtaqueHumano();
  void addSceneAtaque();
  void updateJuegoCpu();
  void ponMisilacoTableroDefensa(tipoPlayer tipo);
  void ponMisilacoTableroAtaque();
  void activaGUINext();
  void createGUINext();
  bool hayGanador();

  CEGUI::MouseButton convertirBotonMouse(OIS::MouseButtonID id);

  bool _exitGame;

 private:
  Ogre::Vector3 _vtCamara;                           //Vector de traslación, en principio no se para que lo voy a usar
  Ogre::Vector3 _vtBarco;
  Ogre::Real _tSpeed;                              //Distancia en unidades del mundo virtual que queremos recorrer en un segundo cuando movamos cositas
  float _rotCamarax;
  float _rotCamaray;
  RaySceneQuery *_raySceneQuery;
  Ray setRayQuery(int posx, int posy, uint32 mask);
  SceneNode *_selectedNode;
  SceneNode *_nodoCursor;
  
  bool _flip;
  int _rotaTablero;
  bool _colocandoBarco;
  
  Ogre::Vector2 extraePosicionCasilla(string nombre);
  void actualizaGUIColocacion(tipoBarco tipo);
  void addBarcoAlTablero(Player *jugador, Barco barco);
  Barco *_barcoSeleccionado;
  std::vector<Player *> _jugadores;
  estadoJuego _estado;
  bool _turnoEnCurso;
  int _idJugador;
  bool _wait;
  
  
  
};

#endif
