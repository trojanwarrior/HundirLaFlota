#include "Cpu.h"

Cpu::Cpu()
{
    inicializaDirecciones();
    _resultado = errado;
    _tipo = cpu;
    _barcoEncontrado = false;
    _puntuacion = 0;
    _contadorHundidos = 0;
    
}

Cpu::~Cpu()
{
    
}

void Cpu::inicializaDirecciones()
{
    while(!_pilaDirecciones.empty())
        _pilaDirecciones.pop();
        
    _pilaDirecciones.push(Ogre::Vector2(-1,0));  //a la derecha
    _pilaDirecciones.push(Ogre::Vector2(1,0));   //a la izquierda
    _pilaDirecciones.push(Ogre::Vector2(0,1));   //abajo
    _pilaDirecciones.push(Ogre::Vector2(0,-1));  //arriba
}

void Cpu::mueve()
{
    actualizaTableroAtaque(_resultado); //nulo es un estado de resultado inicial, así que lo ignoramos.
    switch(_resultado)      // resultado del último tiro, igual a nulo al empezar a jugar.
    {
        case nulo:          ataqueAlAzar(); cout<< "Estado inicial ataqueAlAzar()\n"; break;
        case errado:        if (!_barcoEncontrado)
                                {ataqueAlAzar();  cout <<"Tiro errado: ataqueAlAzar()\n";}
                            else 
                                {sigueAtacando(); cout <<"Tiro errado pero barco a la vista sigueAtacando()\n";}
                            break;
        case tocadoHundido: _barcoEncontrado = false; //el barco que encontramos lo hemos mandado al fondo del mar, luego habrá que encontrar otro.
                            _contadorHundidos++;
                            cout << "Barcos hundidos " << _contadorHundidos;
                            ataqueAlAzar();  cout << "TocadoHundido, ataqueAlAzar()\n"; break;
                            
        case tocado:        _barcoEncontrado = true;
                            sigueAtacando(); cout << "Tocado, sigueAtacando()\n";
    }
    
    
}


tipoPlayer Cpu::getTipoJugador()
{
    return cpu;
}

void Cpu::colocaBarcos()
{
    if (! _barcos.empty())
    {
        std::vector<Barco>::iterator it;
        for (it = _barcos.begin(); it != _barcos.end(); ++it)
            buscarAlojamiento(*it);
    }
}

void Cpu::buscarAlojamiento(Barco &barco)
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
        _casilleroDefensa[i][j]._pieza = barco._piezas[n];
        cout << "[" << i << "," << j << "]";
        barco._piezas[n]._posicion.x = i;
        barco._piezas[n]._posicion.y = j;
        i += direccion.x;
        j += direccion.y;
        
    }
    barco.setOrientacion((direccion.x)?horizontal:vertical);
}

void Cpu::sigueAtacando()
{
    
    
    /// PELIAGUDO :D
    _casillaTiroAnterior = _casillaTiro; // Me guardo el acierto anterior
    
    do
    {
        if (_resultado == errado) // Si ultimo resultado es errado, pero entro aquí es que tenía un barco en el punto de mira, luego
        {
            _casillaTiro = _casillaOrigen;  // me coloco en la casilla en la que encontré ese barco y
            _pilaDirecciones.pop();         // la dirección que llevaba ya no es válida pues erré.
        }
        
        if (!_pilaDirecciones.empty()) // Si no he agotado todas las direcciones
        {
            // Compruebo si mi dirección actual me saca del tablero, si me saca, quito esa dirección y me coloco en la casilla
            // donde acerté inicialmente.
            Ogre::Vector2 dirActual = _pilaDirecciones.top();
            while (!inBounds(_casillaTiro + _pilaDirecciones.top())) 
                _pilaDirecciones.pop();
            if (dirActual != _pilaDirecciones.top())
                _casillaTiro = _casillaOrigen;
            
            _casillaTiro += _pilaDirecciones.top();
        }
        else // Si por el contrario he agotado las direcciones posibles. A la porra, tiramos un dado :D
        {
            ataqueAlAzar();
            break;  // Forzamos la salida del while pues ataqueAlAzar() ya se preocupa de proveer una casilla sin explorar.
        }
    }
    while (_casilleroAtaque[_casillaTiro.x][_casillaTiro.y]._estado != neutro);
}

bool Cpu::inBounds(Ogre::Vector2 posicion)
{
    return (posicion.x >= 0 && posicion.x <=9 && posicion.y >= 0 && posicion.y <= 9);
}

void Cpu::ataqueAlAzar()
{
    bool casillaSinExplorar = false;
    int x, y;
    
    //Un ataque al azar implica reiniciar la pila de direcciones.
    inicializaDirecciones();
    
    while (!casillaSinExplorar)
    {
        try
        {
        x = rand() % 9;
        y = rand() % 9;
        
        if (_casilleroAtaque[x][y]._estado == neutro)
            casillaSinExplorar = true;
        
        }
        catch(...)
        {
            cout << "mierda!!!";
        }
    }
    
    _casillaTiro = Ogre::Vector2(x,y);
    // Cuando tire al azar casillaOrigen y casillaTiro valdrán lo mismo. Así, si el tiro acierta ya tendré guardada
    // la casilla en la que empecé a acertar.
    _casillaOrigen = _casillaTiro;  
}