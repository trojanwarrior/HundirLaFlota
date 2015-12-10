#ifndef CPU_H
#define CPU_H

#include "Player.h"
#include <stack>


class Cpu : public Player
{
    
    public:
       Cpu();
       ~Cpu();
       void mueve();
       void colocaBarcos();
       void buscarAlojamiento(Barco barco);
       
       bool _haTocado;
       bool _haHundido;
       Ogre::Vector2 _casillaAnterior;
       Ogre::Vector2 _casillaTiro;
       Ogre::Vector2 _casillaOrigen;
       
    private:
        void sigueAtacando();
        void ataqueAlAzar();
        bool inBounds(Ogre::Vector2 posicion);
        
        std::stack<Ogre::Vector2> _pilaDirecciones;
        void inicializaDirecciones();
       
};

#endif