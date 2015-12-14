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
       tipoPlayer getTipoJugador();
       resultadoTiro resultadoTiroOponente(Ogre::Vector2 tiro);
       void buscarAlojamiento(Barco &barco);
       bool _barcoEncontrado;
       
    private:
        void sigueAtacando();
        void ataqueAlAzar();
        bool inBounds(Ogre::Vector2 posicion);
        
        
        std::stack<Ogre::Vector2> _pilaDirecciones;
        void inicializaDirecciones();
       
};

#endif