#include "Casilla.h"

//Constructor de copia
Casilla::Casilla(const Casilla &obj)
{
    _posicion = obj._posicion;
    _estado = obj._estado;
    _vacia = obj._vacia;
    if (obj._pieza)
    {
        _pieza = new Pieza();
        _pieza->_posicion = obj._posicion;
        _pieza->_tocada = obj._pieza->_tocada;
    }
    else
        _pieza = NULL;
    
}

//Sobrearga de asignación
Casilla& Casilla::operator=(const Casilla &obj)
{
    _posicion = obj._posicion;
    _estado = obj._estado;
    _vacia = obj._vacia;

    if (obj._pieza)
    {
        _pieza = new Pieza();
        _pieza->_posicion = obj._posicion;
        _pieza->_tocada = obj._pieza->_tocada;
    }
    else _pieza = NULL;
    
    return *this;
}


Casilla::~Casilla()
{
    if (_pieza) 
        delete _pieza;
}

