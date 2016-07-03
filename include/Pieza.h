#ifndef Pieza_H
#define Pieza_H

#include <Ogre.h>

using namespace Ogre;

class Pieza
{
    public:
        Pieza() : _posicion(Ogre::Vector2(0,0)), _tocada(false){};
        Pieza(Ogre::Vector2 posicion, bool tocada) : _posicion(posicion), _tocada(tocada){};
        //Pieza(){};
        
        //Constructor de copia
        Pieza(const Pieza &obj);
        //Sobrecarga operador de asignacion
        Pieza& operator=(const Pieza &obj);
        
        
        ~Pieza(){};
        
        Ogre::Vector2 _posicion;
        bool _tocada;
};

#endif