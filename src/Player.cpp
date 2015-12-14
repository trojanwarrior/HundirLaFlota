#include "Player.h"
#include "Casilla.h"

void Player::AddBarco(tipoBarco tipo)
{
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
        if (!_barcos[i]._hundido)
        {
            for (size_t j=0; j<_barcos[i]._piezas.size(); j++)
                if (_barcos[i]._piezas[j]._posicion == tiro)
                {
                    _barcos[i]._piezas[j]._tocada = true;
                    res = tocado; break;
                
                }
            if (!_barcos[i].getPiezasSinTocar())
            {
                _barcos[i]._hundido = true;
                res = tocadoHundido; break; // Si lo hemos hundido ya no seguimos mirando mas barcos
            }
            
        }
    }           
    
    return res;

}


estadoCasilla Player::convierteEstadoTiroAEstadoCasilla(resultadoTiro res)
{
    switch (res)
    {
        case errado: return agua;
        case nulo: return aSaber;
        case tocado:
        case tocadoHundido: return acierto;
    }
    
    return neutro;
}

void Player::actualizaTableroAtaque(resultadoTiro res)
{
    if (res != nulo)
        _casilleroAtaque[_casillaTiro.x][_casillaTiro.y]._estado = convierteEstadoTiroAEstadoCasilla(res);
}

void Player::coutTableroDefensa()
{
      cout << "\n\n";
      cout << "Casillero Defensa " << getNombreTipoJugador() << endl;
      for (size_t y = 0; y < _casilleroDefensa.size(); y ++)
      {    
            cout << "\n";
            for (size_t x = 0; x < _casilleroDefensa[y].size(); x ++)
                cout << _casilleroDefensa[x][y]._vacia << " ";
      }
      cout << "\n\n";
}

void Player::coutTableroAtaque()
{
      cout << "\n\n";
      cout << "Casillero ataque " << getNombreTipoJugador() << endl;
      for (size_t y = 0; y < _casilleroAtaque.size(); y ++)
      {    
            cout << "\n";
            for (size_t x = 0; x < _casilleroAtaque[y].size(); x ++)
                cout << codigoEstadoCasilla(_casilleroAtaque[x][y]._estado) << " ";
      }
      cout << "\n\n";
}


char Player::codigoEstadoCasilla(estadoCasilla e)
{
    switch(e)
    {
        case neutro: return 'N';
        case agua: return 'A';
        case acierto: return 'T';
        default: return '?';
    }
}

const char * Player::getNombreTipoJugador()
{
    switch (getTipoJugador())
    {
        case humano: return "humano";
        case cpu: return "cpu";
        default: return "";
    }
}

bool Player::hePerdido()
{
    for (size_t i=0; i<_barcos.size(); ++i)
        if (!_barcos[i]._hundido)
            return false;
        
    return true;
}



