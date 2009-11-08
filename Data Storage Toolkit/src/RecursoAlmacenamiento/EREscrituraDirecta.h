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
	EREscrituraDirecta(Almacenamiento*buffer,size_t cantidadElementoBuffer);
	virtual ~EREscrituraDirecta();
	virtual bool insertar(Registro* registro);
	virtual bool eliminar(Clave* unaClave);
	virtual bool modificar(Clave* unaClave,Registro* registro);
	virtual bool obtener(Clave* unaClave,Registro*registro);
	virtual EstrategiaAlmacenamiento *getEstrategiaAlmacenamiento() const;
	virtual EstrategiaAlmacenamiento *getEstrategiaBuffer() const;
	virtual EstrategiaIndice *getIndice() const;
	virtual Registro *getRegistro() const;
	virtual void setEstrategiAlmacenamiento(EstrategiaAlmacenamiento *estrategiAlmacenamiento);
	virtual void setIndice(EstrategiaIndice *indice);
	virtual void setRegistro(Registro *registro);
	Almacenamiento* getBuffer();
};

#endif // ESTRATEGIARECURSOSESCRITURADIRECTA_H
