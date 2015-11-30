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

#ifndef IntroState_H
#define IntroState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"

#define STAGE   1 << 0 // mascara para el escenario
#define BARCO   1 << 1 // mascara para un barco, en realidad será una casilla donde haya un trozo de barco
#define CASILLA 1 << 2


class IntroState : public Ogre::Singleton<IntroState>, public GameState
{
 public:
  IntroState() {};

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
  bool quit(const CEGUI::EventArgs &e);


  // Heredados de Ogre::Singleton.
  static IntroState& getSingleton ();
  static IntroState* getSingletonPtr ();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  CEGUI::OgreRenderer* _renderer;
  Ogre::Real _deltaT;

  void createGUI();
  void createScene();

  CEGUI::MouseButton convertirBotonMouse(OIS::MouseButtonID id);

  bool _exitGame;

 private:
  int _mecer;
  Ogre::Vector3 _vtCamara;                           //Vector de traslación, en principio no se para que lo voy a usar
  Ogre::Vector3 _vtBarco;
  Ogre::Real _tSpeed;                              //Distancia en unidades del mundo virtual que queremos recorrer en un segundo cuando movamos cositas
  int _r; // para rotar sobre eje Y al barquito
  float _rotCamarax;
  float _rotCamaray;
  RaySceneQuery *_raySceneQuery;
  Ray setRayQuery(int posx, int posy, uint32 mask);
  SceneNode *_selectedNode;

};

#endif
