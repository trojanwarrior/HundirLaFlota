#include "PlayManager.h"

PlayManager::PlayManager()
{
    //_jugadores.push_back(new Player());
}

void PlayManager::addPlayer(Player *player)
{
         _jugadores.push_back(player);
        
}

PlayManager::~PlayManager()
{
}

void PlayManager::start()
{
}

void PlayManager::inicializarPartida()
{
}

void PlayManager::cambiaTurno()
{
}

bool PlayManager::hayGanador()
{
    return false;
}

void PlayManager::reseteaPartida()
{
}

void PlayManager::terminaPartida()
{
}

void PlayManager::guardaRecords()
{
}
