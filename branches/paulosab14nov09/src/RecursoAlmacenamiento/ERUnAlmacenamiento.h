
#ifndef ESTRATEGIARECURSOSUNALMACENAMIENTO_H
#define ESTRATEGIARECURSOSUNALMACENAMIENTO_H
#include "EstrategiaRecursos.h"

#include <string>

class ERUnAlmacenamiento : public EstrategiaRecursos{
public:
	ERUnAlmacenamiento (EstrategiaIndice*indice,EstrategiaAlmacenamiento *estrategiAlmacenamiento);
	virtual ~ERUnAlmacenamiento ( );
	bool insertar(Registro* registro);
	bool eliminar(Clave* unaClave);
	bool modificar(Clave* unaClave,Registro* registro);
	bool obtener(Clave* unaClave,Registro*registro);
    EstrategiaAlmacenamiento *getEstrategiaAlmacenamiento() ;
    EstrategiaIndice *getIndice() ;
    void setEstrategiAlmacenamiento(EstrategiaAlmacenamiento *estrategiAlmacenamiento);
    void setIndice(EstrategiaIndice *indice);
private:
    void setClave(Registro*reg,Clave*clave);
	void actualizarIndice(Cambio cambio);
	EstrategiaAlmacenamiento* estrategiAlmacenamiento;
	EstrategiaIndice* indice;
	Registro*registro;
	Clave*clave;
};

#endif // ESTRATEGIARECURSOSUNALMACENAMIENTO_H
