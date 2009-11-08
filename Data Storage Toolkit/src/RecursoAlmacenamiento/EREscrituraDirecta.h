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
	AdministradorDeBuffer();
	virtual ~AdministradorDeBuffer();
	bool buscar(size_t posicionArchivo);
	size_t getPosicionEnBuffer();
	size_t size();
	NodoArchivoBuffer* at(size_t pos);
	bool acceder(size_t posicionArchivo);
	void insertar(size_t posicionArchivo);
	friend void imprimir2(AdministradorDeBuffer &admin);
	friend void imprimir(AdministradorDeBuffer &admin);
	void setCapacidad(size_t capacidad){
		this->capacidad=capacidad;
	};
};

class EREscrituraDirecta : public EstrategiaRecursos{

private:
	AdministradorDeBuffer admin;
	EstrategiaAlmacenamiento*estrategiaArchivo;
	EstrategiaAlmacenamiento*estrategiaBuffer;
	EstrategiaIndice* indice;
	Registro*registro;
	Clave*clave;
	void actualizarIndice(Cambio cambio);
	void actualizarBuffer(Cambio cambio);
	void insertarEnBuffer(Referencia refArchivo);
	void setClave(Registro*reg,Clave*clave);
public:
	EREscrituraDirecta(EstrategiaIndice* indice,EstrategiaAlmacenamiento*estrategiaArchivo,EstrategiaAlmacenamiento*estrategiaBuffer,size_t cantidadElementoBuffer);
	virtual ~EREscrituraDirecta();
	bool insertar(Registro* registro);
	bool eliminar(Clave* unaClave);
	bool modificar(Clave* unaClave,Registro* registro);
	bool obtener(Clave* unaClave,Registro*registro);
	EstrategiaAlmacenamiento *getEstrategiaAlmacenamiento();
	EstrategiaAlmacenamiento *getEstrategiaBuffer();
	EstrategiaIndice *getIndice();
	void setEstrategiAlmacenamiento(EstrategiaAlmacenamiento *estrategiAlmacenamiento);
	void setIndice(EstrategiaIndice *indice);
	void setEstrategiaBuffer(EstrategiaAlmacenamiento* estrategia);
};

#endif // ESTRATEGIARECURSOSESCRITURADIRECTA_H
