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
	EstrategiaAlmacenamiento*estrategiaArchivo;
	EstrategiaAlmacenamiento*estrategiaBuffer;
	Almacenamiento* archivo;
	EstrategiaIndice* indice;
	Registro*registro;
	Clave*clave;
	size_t posicionEnBuffer(size_t posicionArchivo);
	void actualizarTabla(Cambio cambio);
	void actualizarIndice(Cambio cambio);
public:
	EREscrituraDirecta(Almacenamiento*buffer);
	virtual ~EREscrituraDirecta();
	virtual bool insertar(Registro* registro);
	virtual bool eliminar(Clave* unaClave);
	virtual bool modificar(Clave* unaClave,Registro* registro);
	virtual bool obtener(Clave* unaClave,Registro*registro);
};

#endif // ESTRATEGIARECURSOSESCRITURADIRECTA_H
