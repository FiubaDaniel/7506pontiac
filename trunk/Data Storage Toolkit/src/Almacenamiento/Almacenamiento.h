
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
  virtual void escribir(std::istream&entrada,size_t cantidad)=0;
  virtual void escribir(const void* bytes,size_t cantidad)=0;
  virtual void escribir(const void* unByte)=0;
  virtual void leer(std::ostream&salida,size_t cantidad)=0;
  virtual void leer(void* bytes,size_t cantidad)=0;
  virtual void leer(void* unBytes)=0;
  virtual void posicionarByte(size_t posicion)=0;
  virtual bool bienByte()=0;
  /*------------------------------------------------------------------------*/
  //TODO usar un unico posicionamiento o usar uno para lectura y otro para escritura?
  virtual void posicionarCompuesto(size_t nroCompuesto)=0;
  virtual void escribir(Compuesto*compuesto)=0;
  virtual void leer(Compuesto*compuesto)=0;
  virtual bool fin()=0;
  virtual bool bien()=0;
  //TODO que mas hace falta?
};

#endif // ALMACENAMIENTO_H
