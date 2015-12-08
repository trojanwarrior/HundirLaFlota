#include "Barco.h"


Barco::Barco(tipoBarco tipo)
{
    _tipo = tipo;
     //Creamos un barco mediante el tipo pasado
     _piezas.reserve(tipo);
     for (size_t i = 0; i<tipo; i++)
         _piezas.push_back(new Pieza());
     
}

//Constructor copia
Barco::Barco(const Barco& obj)
{
    _tipo = obj._tipo;
    
    if (_piezas.size()) _piezas.clear();
    
    _piezas.reserve(_tipo);
    
    for (size_t i = 0; i<_tipo; i++)
        _piezas.push_back(obj._piezas[i]);
    
}

//Sobrecarga asignacion
Barco& Barco::operator =(const Barco &obj)
{
    
    if (_piezas.size() < obj._piezas.size())
        _piezas.clear();

    return *this;
}


Barco::~Barco()
{
    for (size_t i = 0; i < _piezas.size(); i++)
        if (_piezas[i]) delete _piezas[i];
    
}

tipoBarco Barco::getTipo()
 {
    return _tipo;
 }
        
int Barco::getSizeEnPiezas()
{
        return _piezas.size();
}

string Barco::getNombreTipo()
{
        switch (_tipo)
        {
            case portaviones: return "portaviones";
            case acorazado: return "acorazado";
            case lancha: return "lancha";
            default: return "";
        }
}

int Barco::getPiezasSinTocar ()
{
    int aux = 0;
    std::vector<Pieza>::size_type sz = _piezas.size();
    for (std::size_t i=0; i<sz; i++)
        if (!_piezas[i]->_tocada) aux++;
    
    return aux;
}



