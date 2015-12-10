#include "PlayManager.h"


void PlayManager::addPlayer(Player *player)
{
     _jugadores.push_back(player);
}

PlayManager::~PlayManager()
{
}

void PlayManager::start()
{
    _enJuego = true;
    cambiaTurno();
}

void PlayManager::inicializarPartida()
{
    _turno = _jugadores.size();
    
}

void PlayManager::cambiaTurno()
{
    _turno = _turno % _jugadores.size();
    _turnoEnCurso = true;
    _jugadores[_turno]->mueve();
}

bool PlayManager::hayGanador()
{
    for(size_t i = 0; i < _jugadores.size(); ++i)
        if (_jugadores[i]->_barcos.empty()) return true;
        
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

void PlayManager::espera(bool wait)
{
    _colocando = wait;
}

bool PlayManager::espera()
{
    return _colocando;
}