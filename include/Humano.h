#ifndef HUMANO_H
#define HUMANO_H

#include "Player.h" // Base class: Player

class Humano : public Player
{
public:
    Humano();
    //~Humano();

    void mueve();
    bool colocaBarcos(Ogre::Vector2 posCasilla, Barco &barco);
    
    
    
};

#endif // HUMANO_H
