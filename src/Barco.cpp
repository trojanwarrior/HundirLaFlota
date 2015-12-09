#include "Barco.h"


Barco::Barco(tipoBarco tipo)
{
    _tipo = tipo;
     //Creamos un barco mediante el tipo pasado
     _piezas.reserve(tipo);
     for (size_t i = 0; i<tipo; i++)
         _piezas.push_back(Pieza());
     
}

//Constructor copia
Barco::Barco(const Barco& obj)
{
    _tipo = obj._tipo;
    _orientado = obj._orientado;
    _piezas = obj._piezas;
    
}

//Sobrecarga asignacion
Barco& Barco::operator =(const Barco &obj)
{
    
    _piezas = obj._piezas;
    _tipo = obj._tipo;
    _orientado = obj._orientado;

    return *this;
}


Barco::~Barco()
{

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
        if (!_piezas[i]._tocada) aux++;
    
    return aux;
}

orientacion Barco::getOrientacion()
{
    return _orientado;
}
        
void Barco::setOrientacion(orientacion ori)
{
    _orientado = ori;
}




