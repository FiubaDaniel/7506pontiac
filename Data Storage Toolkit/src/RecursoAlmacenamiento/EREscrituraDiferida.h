
#ifndef ESTRATEGIARECURSOSESCRITURADIFERIDA_H
#define ESTRATEGIARECURSOSESCRITURADIFERIDA_H
#include "EstrategiaRecursos.h"
#include "BufferCache.hpp"

#include <string>

class EREscrituraDiferida : public EstrategiaRecursos{


private:

        BufferCache buffer_recurso;
        EstrategiaIndice* indice;

public:
        EREscrituraDiferida (EstrategiaIndice *indice ):buffer_recurso() {
                        this->indice = indice;
        };
        virtual ~EREscrituraDiferida ( ){};

        bool insertar(Registro* registro);

        bool eliminar(Clave* unaClave);

        bool modificar(Clave* unaClave,Registro* registro);

        bool obtener(Clave* unaClave,Registro*registro);
};

#endif // ESTRATEGIARECURSOSESCRITURADIFERIDA_H
