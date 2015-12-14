#ifndef PLAYER_H
#define PLAYER_H

#include "Casilla.h"
#include "Barco.h"

using namespace std;

typedef std::vector< std::vector<Casilla> > Casillero_t;

enum tipoPlayer
{
    humano,
    cpu,
    unknown
};

enum resultadoTiro
{
    errado,  //inicializamos con este resultado, así forzamos a la CPU a empezar con un tiro aleatorio
    tocado,
    tocadoHundido,
    nulo
};

class Player
{
public:

    
    static const int NUM_MAX_LANCHAS = 5;
    static const int NUM_MAX_ACORAZADOS = 3;
    static const int NUM_MAX_PORTAVIONES = 2;
    
    Player() : _casilleroAtaque(10, std::vector<Casilla>(10)), _casilleroDefensa(10, std::vector<Casilla>(10)){};
    ~Player(){};
    //Constructor de copia
    Player(const Player &obj);
    //Sobrecarga de asignación
    Player& operator=(const Player &obj);
    
    
    virtual void mueve() {cout << "metodo virtual 'mueve' clase base, no hace nada \n";}
    virtual void colocaBarcos() { cout << "metodo virtual 'colocaBarcos' clase base, no hace nada \n"; }
    virtual tipoPlayer getTipoJugador(){return unknown;}
    const char* getNombreTipoJugador();
    
    resultadoTiro resultadoTiroOponente(Ogre::Vector2 tiro);
    estadoCasilla convierteEstadoTiroAEstadoCasilla(resultadoTiro res);
    void actualizaTableroAtaque(resultadoTiro res);
    void coutTableroDefensa();
    void coutTableroAtaque();
    char codigoEstadoCasilla(estadoCasilla e);
    void AddBarco(tipoBarco tipo);
    bool hePerdido();

    std::vector<Barco> _barcos;
    Casillero_t _casilleroAtaque;
    Casillero_t _casilleroDefensa;

    int _puntuacion;
    int _num_lanchas;
    int _num_acorazados;
    int _num_portaviones;
    tipoPlayer _tipo;
    resultadoTiro _resultado;
    
    Ogre::Vector2 _casillaOrigen;           //Posición de la casilla donde tiramos al principio de una jugada, coincidirá con _casillaTiro al principio de una jugada
    Ogre::Vector2 _casillaTiro;             //Posición elegida para hacer un tiro.
    Ogre::Vector2 _casillaTiroAnterior;     //Posición de la casilla del tiro anterior, para tirar para atrás en caso necesario.

};

#endif // PLAYER_H
