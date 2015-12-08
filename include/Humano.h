#ifndef HUMANO_H
#define HUMANO_H

#include "Player.h" // Base class: Player

class Humano : public Player
{
public:
    /*Humano(){};
    ~Humano();*/

    virtual void mueve();
};

#endif // HUMANO_H
