
#ifndef ESTRATEGIARECURSOSESCRITURADIFERIDA_H
#define ESTRATEGIARECURSOSESCRITURADIFERIDA_H
#include "EstrategiaRecursos.h"
#include "BufferCache.hpp"
#include "../Almacenamiento/Almacenamiento.h"
#include <string>

class EREscrituraDiferida : public EstrategiaRecursos{


private:

        BufferCache buffer_recurso;
        EstrategiaIndice *indice;
        Almacenamiento *almacen;
        std::queue<Cambio> cambiosArchivo;

public:
        EREscrituraDiferida (EstrategiaIndice *indice, Almacenamiento *almacen ):buffer_recurso(almacen) {
                        this->indice = indice;
                        this->almacen = almacen;
        };
        virtual ~EREscrituraDiferida ( ){};

        bool insertar(Registro* registro);

        bool eliminar(Clave* unaClave);

        bool modificar(Clave* unaClave,Registro* registro);

        bool obtener(Clave* unaClave,Registro*registro);
};

#endif // ESTRATEGIARECURSOSESCRITURADIFERIDA_H
