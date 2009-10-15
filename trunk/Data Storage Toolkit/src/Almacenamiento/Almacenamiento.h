
#ifndef ALMACENAMIENTO_H
#define ALMACENAMIENTO_H
#include "../Compuesto/Compuesto.h"
#include "EstrategiaAlmacenamiento.h"
/**
  * class Almacenamiento
  * 
  */
class EstrategiaAlmacenamiento;
class Almacenamiento{
	EstrategiaAlmacenamiento* estrategiaUsada;
public:
  Almacenamiento (){};
  virtual ~Almacenamiento ( ){};
  /*-------------------------------------------------------------------------*/
  /*utilizado por la estrategiaAlmacenamiento*/
  virtual void escribir(const void* bytes,size_t cantidad)=0;
  virtual void escribir(const void* unByte)=0;
  virtual void leer(void* bytes,size_t cantidad)=0;
  virtual void leer(void* unBytes)=0;
  virtual void posicionarByte(size_t posicion)=0;
  virtual void posicionarAlfinal()=0;
  virtual size_t posicionActual()=0;
  virtual bool bien()=0;
  virtual bool fin()=0;
};

#endif // ALMACENAMIENTO_H

