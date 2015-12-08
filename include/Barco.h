#ifndef BARCO_H
#define BARCO_H

#include <string>
#include <vector>
#include "Pieza.h"

using namespace std;

  enum tipoBarco
        {
            portaviones = 5, // 5 piezas
            acorazado = 3,   // 4 piezas
            lancha  = 1,       // 2 piezas
            sinTipo = 0
        };

class Barco
{
    public:
        
        Barco() : _tipo(sinTipo){};
        Barco(tipoBarco tipo);
        
        //Constructor copia
        Barco(const Barco &obj);
        //Sobrecarga asignación
        Barco &operator=(const Barco &obj);
                
        ~Barco();
        
        tipoBarco getTipo();
        
        int getSizeEnPiezas();
        string getNombreTipo();
        int getPiezasSinTocar ();
        
        std::vector<Pieza *> _piezas;
    
    
private:
    tipoBarco _tipo;
    
    
    

};

#endif // BARCO_H
