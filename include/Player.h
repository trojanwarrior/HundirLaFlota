#ifndef PLAYER_H
#define PLAYER_H

#include "Casilla.h"
#include "Barco.h"

using namespace std;

typedef std::vector< std::vector<Casilla> > tCasillero;
//typedef std::vector<Casilla> tCasillero;


class Player
{
public:
    Player() : _casilleroAtaque(10, std::vector<Casilla>(10)), _casilleroDefensa(10, std::vector<Casilla>(10)){};
    //Player() : _casilleroAtaque(std::vector<Casilla>(10)), _casilleroDefensa(std::vector<Casilla>(10)){};
    ~Player();
    
    //Constructor de copia
    Player(const Player &obj);
    
    //Sobrecarga de asignación
    Player& operator=(const Player &obj);
    
    
    virtual void mueve() {cout << "metodo virtual 'mueve' clase base, no hace nada \n";}
    
    void AddBarco(tipoBarco tipo);

    std::vector<Barco *> _barcos;
    tCasillero _casilleroAtaque;
    tCasillero _casilleroDefensa;

    int _puntuacion;


protected:

    //typedef std::vector<Casilla> filaCasillas[10];
    //std::vector<filaCasillas> _casilleroDefensa[10];
    //std::vector<filaCasillas> _casilleroAtaque[10];
    //std::vector< std::vector<Casilla> > _casilleroDefensa[10][10];
    //std::vector< std::vector<Casilla> > _casilleroAtaque[10][10];
    

};

#endif // PLAYER_H
