#ifndef CPU_H
#define CPU_H

#include "Player.h"

class Cpu : public Player
{
    public:
       void mueve();
       void colocaBarcos();
       void buscarAlojamiento(Barco barco);
};

#endif