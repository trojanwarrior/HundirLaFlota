#include "Cpu.h"


void Cpu::mueve()
{
}

void Cpu::colocaBarcos()
{
    if (! _barcos.empty())
    {
        std::vector<Barco>::iterator it;
        for (it = _barcos.begin(); it != _barcos.end(); ++it)
        {
            for (size_t y = 0; y < _casilleroDefensa.size(); y ++)
            {    
                cout << "\n";
                for (size_t x = 0; x < _casilleroDefensa[y].size(); x ++)
                    cout << _casilleroDefensa[x][y]._vacia << " ";
            }
            cout << "\n\n";
            
            buscarAlojamiento(*it);
            
        }
        
        
        
    }
}

void Cpu::buscarAlojamiento(Barco barco)
{
    bool colocado = false;
    int i = rand() % 9;
    int j = rand() % 9;
    int auxJ = j;
    int auxI = i;
    int espacio = barco.getSizeEnPiezas() - 1; // -1 por que la casilla actual también cuenta.
    Ogre::Vector2 direccion = Ogre::Vector2(0,0); // primero quietecito
    
    while (!colocado)
    {
        cout << "\n" << i << "," << j << endl;
        int horVer = rand() % 2;  // De entrada decidimos si buscar en vertical o en horizontal
        
        if (horVer) //vertical
        {
            if (j + espacio < 10) // Miramos hacia un abajo
            {
                direccion = Ogre::Vector2(0,1); 
                for (  ;  j <= auxJ + espacio ;  ++j)
                    if (!_casilleroDefensa[i][j]._vacia)
                    {
                        colocado = false; break; // Con que encontremos una ocupada, ya no seguimos
                    }
                    else
                        colocado = true; // Mientras la casilla que estemos mirando esté libre seguimos
            }
            else // o hacia arriba, en algún sitio tendrá que caber 
            {
                direccion = Ogre::Vector2(0,-1); // 
                for (  ;  j <= auxJ - espacio;  --j)
                    if (!_casilleroDefensa[i][j]._vacia)
                    {
                        colocado = false;
                        break;
                    }
                    else
                        colocado = true;
            }
        }
        else // horizontal, o simplemente acabe entrando por azar aquí por que no nos cabía al mirar en horizontal
            if (!colocado)
            {
              j = auxJ; // recuperamos la j original
              if (i + espacio < 10)  // Miramos a la derecha
              {
                direccion = Ogre::Vector2(1,0);   
                for (  ;  i <= auxI + espacio ;  ++i)
                    if (!_casilleroDefensa[i][j]._vacia)
                    {
                        colocado = false;         break; //Con que encontremos una ocupada ya no seguimos 
                    }
                    else
                        colocado = true; // Mientras la casilla que estemos mirando esté libre seguimos.
                        
              }
              else // o a la izquierda, de nuevo hacia algún lado tendrá que caber
              {
                  direccion = Ogre::Vector2(-1,0);
                    for (  ;  i <= auxI - espacio;  --i)
                        if (!_casilleroDefensa[i][j]._vacia)
                        {
                            colocado = false;
                            break;
                        }
                        else
                            colocado = true;
              }
              
            }
                 
        // Si entro aquí es que la primera que he mirado ya estaba ocupada, o aun estando libre no había huecos adyacentes,
        // seguimos buscando una para empezar.
        if (!_casilleroDefensa[auxI][auxJ]._vacia || !colocado)
        {    
            i = rand() % 9;
            j = rand() % 9;
            auxJ = j;
            auxI = i;
        }   
    }
    
    //Finalmente encontramos una casilla libre así como espacio adyacente libre.
    i = auxI;
    j = auxJ;
    cout << "\nBarco " << barco.getNombreTipo() << ". Posicion piezas: " << endl;
    for (int n = 0; n<=espacio; n++)
    {
        _casilleroDefensa[i][j]._vacia = false;    
        _casilleroDefensa[i][j]._pieza = &barco._piezas[n];
        cout << "[" << i << "," << j << "]";
        barco._piezas[n]._posicion.x = i;
        barco._piezas[n]._posicion.y = j;
        i += direccion.x;
        j += direccion.y;
        
    }
        
    
}