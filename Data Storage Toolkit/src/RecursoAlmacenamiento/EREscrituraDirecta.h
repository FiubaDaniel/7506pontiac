#ifndef ESTRATEGIARECURSOSESCRITURADIRECTA_H
#define ESTRATEGIARECURSOSESCRITURADIRECTA_H
#include "EstrategiaRecursos.h"
#include <map>

/**
 * class EREscrituraDirecta
 *
 */
struct NodoArchivoBuffer;
typedef  std::map<size_t,NodoArchivoBuffer*>::iterator PNodoSiguiente;

struct NodoArchivoBuffer{
	size_t posicionBuffer;
	PNodoSiguiente siguiente;
};
class AdministradorDeBuffer{
private:
	PNodoSiguiente posicionEnTabla;
	size_t capacidad;
	void promover(PNodoSiguiente &promovido);
	void promoverAprimero(PNodoSiguiente &promovido);
	std::map<size_t,NodoArchivoBuffer*> tablaArchivoBuffer;
public:
	PNodoSiguiente tope;
	AdministradorDeBuffer(size_t capacidad);
	virtual ~AdministradorDeBuffer();
	bool buscar(size_t posicionArchivo);
	size_t getPosicionEnBuffer();
	size_t size();
	NodoArchivoBuffer* at(size_t pos);
	bool acceder(size_t posicionArchivo);
	void insertar(size_t posicionArchivo);
	friend void imprimir2(AdministradorDeBuffer &admin);
	friend void imprimir(AdministradorDeBuffer &admin);
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
	//size_t posicionEnBuffer(size_t posicionArchivo);
	void actualizarIndice(Cambio cambio);
	void actualizarBuffer(Cambio cambio);
	void insertarEnBuffer(Referencia refArchivo);
	void setClave(Registro*reg,Clave*clave);
public:
	EREscrituraDirecta(Almacenamiento*buffer);
	virtual ~EREscrituraDirecta();
	virtual bool insertar(Registro* registro);
	virtual bool eliminar(Clave* unaClave);
	virtual bool modificar(Clave* unaClave,Registro* registro);
	virtual bool obtener(Clave* unaClave,Registro*registro);
};

#endif // ESTRATEGIARECURSOSESCRITURADIRECTA_H
