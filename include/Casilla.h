#ifndef CASILLA_H
#define CASILLA_H

#include <Ogre.h>
#include "Pieza.h"

enum estadoCasilla
{
    neutro,
    agua,
    acierto,
    aSaber
};

class Casilla
{
public:
    Casilla() : _posicion(0,0), _estado(neutro), _pieza(NULL), _vacia(true) {};
    Casilla(Ogre::Vector2 posicion, estadoCasilla estado, Pieza *pieza, bool vacia) : _posicion(posicion), _estado(estado), _pieza(pieza), _vacia(vacia){};
    Casilla(const Casilla &obj);                            //Constructor de copia
    Casilla& operator=(const Casilla &obj);          //Sobrecarga de asignacion.
    ~Casilla(); //Destructor    
    
    Ogre::Vector2 _posicion;
    estadoCasilla _estado;
    Pieza *_pieza;
    bool _vacia;

};

#endif // CASILLA_H
