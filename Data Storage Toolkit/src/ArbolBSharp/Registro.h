
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
	Atributo* get(Ttamanio posicion);
	Atributo* get(std::string nombre);
	Ttamanio  cantidadAtributos();
	Atributo* getId();
	bool esfijo();
public:/*Metodos Heredados*/
	virtual Ttamanio deserializar(std::streambuf&entrada);
	virtual Ttamanio serializar(std::streambuf&salida);
	virtual Ttamanio tamanioSerializado();
	virtual Componente* clonar();
protected:
	Registro();
	std::vector<Atributo*> atributos;
	Ttamanio tamanioBuffer;
	bool fijo;
};

#endif // REGISTRO_H
