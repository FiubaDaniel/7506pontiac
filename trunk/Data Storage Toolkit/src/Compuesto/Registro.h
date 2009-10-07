
#ifndef REGISTRO_H
#define REGISTRO_H
#include "Componente.h"
#include "Atributo.h"
#include <vector>
#include <string>
#include <cstdarg>

/**
  * class Registro
  * 
  */

class Registro : public Componente{
public:
	Registro(unsigned int cantidadAtributos, ... );
	virtual ~Registro();
	virtual Ttamanio deserializar(void*entrada);
	virtual Ttamanio serializar(void*salida);
	virtual Ttamanio tamanioSerializado();
	virtual Componente* clonar();
public:
	virtual Atributo* get(Ttamanio posicion);
	virtual Atributo* get(std::string nombre);
	virtual Ttamanio cantidadAtributos();
	virtual Atributo* getId();
	bool esfijo();
protected:
	Registro();
	std::vector<Atributo*> atributos;
	Ttamanio tamanioBuffer;
	bool fijo;
};

#endif // REGISTRO_H
