#ifndef ESTRATEGIARECURSOSESCRITURADIRECTA_H
#define ESTRATEGIARECURSOSESCRITURADIRECTA_H
#include "EstrategiaRecursos.h"

#include <string>

/**
 * class EREscrituraDirecta
 *
 */

class EREscrituraDirecta : public EstrategiaRecursos{
	Almacenamiento* buffer;
public:
	EstrategiaAlmacenamiento*estrategiaAlmacenamiento;
	Almacenamiento* archivo;
	EstrategiaIndice* indice;
	EstrategiaRecursos* estrategiaUsada;
public:
	EREscrituraDirecta(Almacenamiento*buffer);
	virtual ~EREscrituraDirecta();
	virtual bool agregar(Registro* registro);
	virtual bool eliminar(Clave unaClave);
	virtual bool modificar(Clave unaClave,Registro* registro);
	virtual bool obtener(Clave unaClave,Registro*registro);
};

#endif // ESTRATEGIARECURSOSESCRITURADIRECTA_H
