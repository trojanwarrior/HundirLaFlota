#ifndef PLAYMANAGER_H
#define PLAYMANAGER_H

#include "Player.h"

enum tipoPlayer
{
    humano,
    cpu
};

class PlayManager
{
public:
    PlayManager();
    ~PlayManager();
    
    void start();
    void inicializarPartida();
    void cambiaTurno();
    bool hayGanador();
    void reseteaPartida();
    void terminaPartida();
    void guardaRecords();
    
    void addPlayer(Player *player);
    
    std::vector<Player*> _jugadores;
    
    
    
private:

    

};

#endif // PLAYMANAGER_H
