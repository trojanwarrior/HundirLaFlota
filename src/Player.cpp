#include "Player.h"
#include "Casilla.h"

void Player::AddBarco(tipoBarco tipo)
{
        //_barcos.push_back(new Barco(portaviones));
        _barcos.push_back(new Barco(tipo));
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



