#ifndef BARCO_H
#define BARCO_H

#include <string>
#include <vector>
#include "Pieza.h"

using namespace std;

  enum orientacion
  {
      horizontal = 0,
      vertical = 1
  };
  
  enum tipoBarco
  {
      portaviones = 5, // 5 piezas
      acorazado = 3,   // 3 piezas
      lancha  = 1,     // 1 pieza
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
        orientacion getOrientacion();
        void setOrientacion(orientacion ori);
        
        int getSizeEnPiezas();
        string getNombreTipo();
        int getPiezasSinTocar ();
        
        std::vector<Pieza> _piezas;
    
    
private:
    tipoBarco _tipo;
    orientacion _orientado;
    
    
    

};

#endif // BARCO_H
