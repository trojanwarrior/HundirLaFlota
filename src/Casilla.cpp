#include "Casilla.h"

//Constructor de copia
Casilla::Casilla(const Casilla &obj)
{
    _posicion = obj._posicion;
    _estado = obj._estado;
    _vacia = obj._vacia;
    _pieza = obj._pieza;
}

//Sobrearga de asignación
Casilla& Casilla::operator=(const Casilla &obj)
{
    _posicion = obj._posicion;
    _estado = obj._estado;
    _vacia = obj._vacia;
    _pieza = obj._pieza;
    
    
    return *this;
}


Casilla::~Casilla()
{
}

