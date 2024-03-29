
#ifndef BUFFER_H
#define BUFFER_H
#include "Almacenamiento.h"
#include <sstream>
#include <string>

/**
  * class Buffer
  * 
  */

class Buffer : public Almacenamiento{
	std::stringstream *archivo;
	unsigned longitudBuffer;
public:
	Buffer();
	Buffer(unsigned longitudBuffer);
	Buffer(EstrategiaAlmacenamiento*estrategia,unsigned longitudBuffer);
	Buffer(EstrategiaAlmacenamiento*estrategia);
	~Buffer();
	/*-------------------------------------------------------------------------*/
	/*utilizado por la estrategiaAlmacenamiento*/
	virtual bool abrir(const char* nombre);
	virtual void crear(const char* nombre);
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
	virtual void cerrar();
	virtual Almacenamiento*clonar();
};
#endif // BUFFER_H
