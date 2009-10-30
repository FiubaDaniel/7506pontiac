#ifndef ESTRATEGIARECURSOSESCRITURADIRECTA_H
#define ESTRATEGIARECURSOSESCRITURADIRECTA_H
#include "EstrategiaRecursos.h"
#include <list>

/**
 * class EREscrituraDirecta
 *
 */
struct ParDireccionArchivoBuffer{
	size_t posicionArchivo;
	size_t posicionBuffer;
};
class AdministradorBuffer{
private:
	size_t posicionesTotalesDelBuffer;
	std::vector<ParDireccionArchivoBuffer> tablaArchivoBuffer;
	std::vector<size_t> posicionesUsadasDelArchivo;
	bool buscar(size_t posicionArchivo);
	size_t posicionEnTabla;
public:
	size_t getPosicionEnBuffer();
	bool acceder(size_t posicionArchivo);
	void insertar(size_t posicionArchivo);
};

class EREscrituraDirecta : public EstrategiaRecursos{
private:
	AdministradorBuffer admin;
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
