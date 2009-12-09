
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
        unsigned numeros_bloques;

public:
        EREscrituraDiferida (EstrategiaIndice *indice, Almacenamiento *almacen ):buffer_recurso(almacen) {
                        this->indice = indice;
                        this->almacen = almacen;
                        numeros_bloques = 0;
        };
        virtual ~EREscrituraDiferida ( ){};

        bool insertar(Registro* registro);

        bool eliminar(Clave* unaClave);

        bool modificar(Clave* unaClave,Registro* registro);

        bool obtener(Clave* unaClave,Registro*registro);


        void cerrar(){

                indice->cerrar();
                almacen->cerrar();
        };

    	Almacenamiento *getAlmacenamiento(){return  almacen;};

    	EstrategiaIndice *getIndice(){return indice;};

    	void setIndice(EstrategiaIndice *indice){
                        this->indice = indice;
        };

};

#endif // ESTRATEGIARECURSOSESCRITURADIFERIDA_H
