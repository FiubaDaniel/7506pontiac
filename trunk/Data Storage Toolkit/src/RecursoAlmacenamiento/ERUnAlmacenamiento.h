
#ifndef ESTRATEGIARECURSOSUNALMACENAMIENTO_H
#define ESTRATEGIARECURSOSUNALMACENAMIENTO_H
#include "EstrategiaRecursos.h"

#include <string>

class ERUnAlmacenamiento : public EstrategiaRecursos{
public:
	ERUnAlmacenamiento ( );
	virtual ~ERUnAlmacenamiento ( );
	bool insertar(Registro* registro);
	bool eliminar(Clave* unaClave);
	bool modificar(Clave* unaClave,Registro* registro);
	bool obtener(Clave* unaClave,Registro*registro);
private:
	void actualizarIndice(Cambio cambio);
	EstrategiaAlmacenamiento* estrategiAlmacenamiento;
	EstrategiaIndice* indice;
	Registro*registro;
	Clave*clave;
};

#endif // ESTRATEGIARECURSOSUNALMACENAMIENTO_H
