#ifndef ESTRATEGIARECURSOSESCRITURADIRECTA_H
#define ESTRATEGIARECURSOSESCRITURADIRECTA_H
#include "EstrategiaRecursos.h"
#include <list>

/**
 * class EREscrituraDirecta
 *
 */


struct NodoArchivoBuffer{
	size_t posicionArchivo;
	size_t posicionBuffer;
	NodoArchivoBuffer* siguiente;
};
class AdministradorDeBuffer{
private:
	size_t posicionEnTabla;
	size_t ultimo;
	size_t capacidad;
	void promover(NodoArchivoBuffer*promovido);
	NodoArchivoBuffer* tablaArchivoBuffer;
public:
	NodoArchivoBuffer* tope;
	AdministradorDeBuffer(size_t capacidad);
	virtual ~AdministradorDeBuffer();
	bool buscar(size_t posicionArchivo);
	size_t getPosicionEnBuffer();
	size_t size();
	NodoArchivoBuffer* at(size_t pos);
	bool acceder(size_t posicionArchivo);
	void insertar(size_t posicionArchivo);
};

class EREscrituraDirecta : public EstrategiaRecursos{
private:
	AdministradorDeBuffer admin;
	Almacenamiento* buffer;
	EstrategiaAlmacenamiento*estrategiaArchivo;
	EstrategiaAlmacenamiento*estrategiaBuffer;
	Almacenamiento* archivo;
	EstrategiaIndice* indice;
	Registro*registro;
	Clave*clave;
	size_t posicionEnBuffer(size_t posicionArchivo);
	void actualizarIndice(Cambio cambio);
	void actualizarBuffer(Cambio cambio);
	void insertarEnBuffer(Referencia refArchivo);
public:
	EREscrituraDirecta(Almacenamiento*buffer);
	virtual ~EREscrituraDirecta();
	virtual bool insertar(Registro* registro);
	virtual bool eliminar(Clave* unaClave);
	virtual bool modificar(Clave* unaClave,Registro* registro);
	virtual bool obtener(Clave* unaClave,Registro*registro);
};

#endif // ESTRATEGIARECURSOSESCRITURADIRECTA_H
