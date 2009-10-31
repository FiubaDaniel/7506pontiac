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
	size_t posicionesTotalesDelBuffer;
	NodoArchivoBuffer* tope;
	void promover(NodoArchivoBuffer*promovido);
public:
	AdministradorDeBuffer(size_t maximo);
	bool buscar(size_t posicionArchivo);
	std::vector<NodoArchivoBuffer> tablaArchivoBuffer;
	size_t getPosicionEnBuffer();
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
