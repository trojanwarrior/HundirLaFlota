#ifndef CPU_H
#define CPU_H

#include "Player.h"
#include <stack>


class Cpu : public Player
{
    
    public:
       Cpu();
       ~Cpu();
       void mueve();
       void colocaBarcos();
       resultadoTiro resultadoTiroOponente(Ogre::Vector2 tiro);
       void buscarAlojamiento(Barco barco);
       
       resultadoTiro _resultado;
       bool _barcoEncontrado;
       
    private:
        void sigueAtacando();
        void ataqueAlAzar();
        bool inBounds(Ogre::Vector2 posicion);
        void actualizaTableroAtaque();
        estadoCasilla convierteEstadoTiroAEstadoCasilla(resultadoTiro res);
        std::stack<Ogre::Vector2> _pilaDirecciones;
        void inicializaDirecciones();
       
};

#endif