
#ifndef ARCHIVO_H
#define ARCHIVO_H
#include "Almacenamiento.h"
#include <fstream>
#include <string>

class Archivo : public Almacenamiento{
	std::fstream archivo;
public:
	Archivo(){estrategia=NULL;};
	Archivo (EstrategiaAlmacenamiento* estrategia);
	virtual ~Archivo ();
	/*-------------------------------------------------------------------------*/
	/*utilizado por la estrategiaAlmacenamiento*/
	virtual void crear(const char*ruta);
	virtual bool abrir(const char*ruta);
	virtual void cerrar();
	virtual void escribir(const void* bytes,Referencia cantidad);
	virtual void escribir(const void* unByte);
	virtual void leer(void* bytes,Referencia cantidad);
	virtual void leer(void* unBytes);
	virtual void posicionar(Referencia posicion);
	virtual bool bien();
	virtual bool fin();
	virtual void clear();
	virtual void posicionarAlfinal();
	virtual Referencia posicionActual();
	Almacenamiento* clonar();
};

#endif // ARCHIVO_H
