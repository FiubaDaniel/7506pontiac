
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
public:
	Buffer();
	~Buffer();
	/*-------------------------------------------------------------------------*/
	/*utilizado por la estrategiaAlmacenamiento*/
	virtual void escribir(const void* bytes,size_t cantidad);
	virtual void escribir(const void* unByte);
	virtual void leer(void* bytes,size_t cantidad);
	virtual void leer(void* unBytes);
	virtual void posicionarByte(size_t posicion);
	virtual bool bienByte();
	/*------------------------------------------------------------------------*/
	virtual void posicionarComponente(size_t nroCompuesto);
	virtual void escribir(Compuesto*compuesto);
	virtual void leer(Compuesto*compuesto);
	virtual bool fin();
	virtual bool bien();
};

#endif // BUFFER_H
