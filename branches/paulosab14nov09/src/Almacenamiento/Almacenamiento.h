
#ifndef ALMACENAMIENTO_H
#define ALMACENAMIENTO_H
#include <string>
#include "../Compuesto/Compuesto.h"
#include "EstrategiaAlmacenamiento.h"
/**
  * class Almacenamiento
  * 
  */
class EstrategiaAlmacenamiento;
class Almacenamiento{
protected:
	std::string nombre;
public:
  Almacenamiento (){};
  virtual ~Almacenamiento ( ){};
  std::string getNombre(){return nombre;};
  void setNombre(std::string nombre){
	  this->nombre=nombre;
  };
  /*-------------------------------------------------------------------------*/
  /*utilizado por la estrategiaAlmacenamiento*/
  virtual void escribir(const void* bytes,size_t cantidad)=0;
  virtual void escribir(const void* unByte)=0;
  virtual void leer(void* bytes,size_t cantidad)=0;
  virtual void leer(void* unBytes)=0;
  virtual void posicionar(size_t posicion)=0;
  virtual void posicionarAlfinal()=0;
  virtual size_t posicionActual()=0;
  virtual bool bien()=0;
  /**/
  virtual bool fin()=0;
  /*Limpia los flags de fin de archivo y error*/
  virtual void reiniciar()=0;
  virtual void cerrar()=0;
};

#endif // ALMACENAMIENTO_H

