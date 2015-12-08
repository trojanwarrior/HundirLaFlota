#include "Pieza.h"

//Constructor copia
Pieza::Pieza(const Pieza& obj)
{
    _posicion = obj._posicion;
    _tocada = obj._tocada;
}

//Sobrecarga de asignación.
Pieza& Pieza::operator =(const Pieza &obj)
{
    std::cout <<obj._posicion << std::endl;
    _posicion = obj._posicion;
    _tocada = obj._tocada;
    
    return *this;
    
}
