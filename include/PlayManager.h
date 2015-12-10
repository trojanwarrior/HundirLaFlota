#ifndef PLAYMANAGER_H
#define PLAYMANAGER_H

#include "Player.h"

enum tipoPlayer
{
    humano,
    cpu
};

enum estadoJuego
{
    inicio,
    jugando,
    fin
};

class PlayManager
{
public:
    PlayManager() : _colocando(true), _enJuego(false), _turnoEnCurso(false), _turno(0), _estado(inicio) {};
    ~PlayManager();
    
    void setEstadoJuego(estadoJuego estado);
    estadoJuego getEstadoJuego();
    void start();
    void inicializarPartida();
    void cambiaTurno();
    bool hayGanador();
    void reseteaPartida();
    void terminaPartida();
    void guardaRecords();
    
    void addPlayer(Player *player);
    
    std::vector<Player*> _jugadores;
    
    void espera(bool wait);
    bool espera();
    
    
    bool _colocando;
    bool _enJuego;
    bool _turnoEnCurso;
    int _turno;
    estadoJuego _estado;
    

};

#endif // PLAYMANAGER_H
