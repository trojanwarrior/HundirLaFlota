#include "Player.h"
#include "Casilla.h"

void Player::AddBarco(tipoBarco tipo)
{
        //_barcos.push_back(new Barco(portaviones));
        _barcos.push_back(Barco(tipo));
}   

Player::Player(const Player &obj)
{
    _casilleroAtaque = obj._casilleroAtaque;
    _casilleroDefensa = obj._casilleroDefensa;
    _barcos = obj._barcos;
    _puntuacion = obj._puntuacion;
}

Player& Player::operator=(const Player &obj)
{
    _casilleroAtaque = obj._casilleroAtaque;
    _casilleroDefensa = obj._casilleroDefensa;
    _barcos = obj._barcos;
    _puntuacion = obj._puntuacion;
    
    return *this;
}

resultadoTiro Player::resultadoTiroOponente(Ogre::Vector2 tiro)
{
    resultadoTiro res = errado;
    for (size_t i=0; i<_barcos.size(); i++)
    {
        for (size_t j=0; j<_barcos[i]._piezas.size(); j++)
            if (_barcos[i]._piezas[j]._posicion == tiro)
            {
                _barcos[i]._piezas[j]._tocada = true;
                res = tocado;
            
            }
        if (!_barcos[i].getPiezasSinTocar())
        {
            _barcos[i]._hundido = true;
            res = tocadoHundido;
        }
    }           
    
    return res;

}



