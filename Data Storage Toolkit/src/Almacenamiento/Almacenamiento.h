
#ifndef ALMACENAMIENTO_H
#define ALMACENAMIENTO_H
#include "../Compuesto/Compuesto.h"
#include "EstrategiaAlmacenamiento.h"
/**
  * class Almacenamiento
  * 
  */
class Almacenamiento{
	EstrategiaAlmacenamiento* estrategiaUsada;
public:
  Almacenamiento ();
  virtual ~Almacenamiento ( );
  /*-------------------------------------------------------------------------*/
  /*utilizado por la estrategiaAlmacenamiento*/
  virtual bool escribir(const void* bytes,size_t cantidad)=0;
  virtual bool escribir(const void* unByte)=0;
  virtual bool leer(void* bytes,size_t cantidad)=0;
  virtual bool leer(void* unBytes)=0;
  virtual bool posicionarByte(size_t posicion)=0;
  /*------------------------------------------------------------------------*/
  //TODO usar un unico posicionamiento o usar uno para lectura y otro para escritura?
  virtual bool posicionarComponente(size_t nroCompuesto)=0;
  virtual bool escribir(Compuesto*compuesto)=0;
  virtual bool leer(Compuesto*compuesto)=0;
  //TODO que mas hace falta?
};

#endif // ALMACENAMIENTO_H
