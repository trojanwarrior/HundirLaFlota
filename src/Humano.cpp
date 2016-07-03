#include "Humano.h"

Humano::Humano()
{
    _num_lanchas = 0;
    _num_acorazados = 0;
    _num_portaviones = 0;
    _tipo = humano;
    _puntuacion = 0;

}

void Humano::mueve()
{

}


bool Humano::colocaBarcos(Ogre::Vector2 posCasilla, Barco &barco)
{
    Ogre::Vector2 direccion(0,0);
    int x = posCasilla.x;
    int y = posCasilla.y;
    //En mi juego de barcos, los barcos siempre tienen número impar de piezas, lo cual facilita algunas cosas
    int derredor = barco.getSizeEnPiezas() / 2; // Número de casillas a mirar a los lados
    int auxX = x;
    int auxY = y;
    
    if (_casilleroDefensa[posCasilla.x][posCasilla.y]._vacia)
    {
        if (barco.getTipo() == lancha)
        {
            barco._piezas[0]._posicion = posCasilla;
            _casilleroDefensa[x][y]._vacia = false;
            _casilleroDefensa[x][y]._pieza = barco._piezas[0];
            _barcos.push_back(barco);
            return true;
        }
        else
        {
            bool ok = true;
            switch( barco.getOrientacion())
            {
                case horizontal: // miramos a izquierda y derecha
                     direccion = Ogre::Vector2(1,0);
                     auxX = auxX - derredor;
                     if (x + derredor <= 9 && x - derredor >= 0)
                     {
                         for (int i=x-derredor; i<=x+derredor; ++i)
                             if (!_casilleroDefensa[i][y]._vacia)
                             {  ok = false; break; }// break del bucle for (ojo!)
                     }                   
                     else ok = false;
                     break;
                case vertical: // miramos arriba y abajo
                     direccion = Ogre::Vector2(0,1);
                     auxY = auxY - derredor;
                     if (y + derredor <= 9 && y - derredor >= 0)
                     {    
                         for (int i=y-derredor; i<=y+derredor; ++i)
                             if (!_casilleroDefensa[x][i]._vacia)
                             {  ok = false; break; } // break del bucle for (ojo!)
                     }
                     else ok = false;
                     break;
                default: cout << "\n ¿BARCO SIN ORIENTACION?\n"; return false;
            }
            
            if (ok)
            {
                x = auxX;
                y = auxY;
                for (int n=0; n<barco.getSizeEnPiezas(); ++n)
                {
                    _casilleroDefensa[x][y]._vacia = false;
                    barco._piezas[n]._posicion = Ogre::Vector2(x,y);
                    _casilleroDefensa[x][y]._pieza = barco._piezas[n];
                    x += direccion.x;
                    y += direccion.y;
                }
                _barcos.push_back(barco);
                
                return true;    
            }
        }
    }

    return false;
}


tipoPlayer Humano::getTipoJugador()
{
    return humano;
}