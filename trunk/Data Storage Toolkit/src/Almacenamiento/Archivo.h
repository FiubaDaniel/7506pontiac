
#ifndef ARCHIVO_H
#define ARCHIVO_H
#include "Almacenamiento.h"
#include <fstream>
#include <string>

class Archivo : public Almacenamiento{
	std::fstream archivo;
public:
  Archivo (const char*ruta);
  virtual ~Archivo ( );
  /*-------------------------------------------------------------------------*/
  /*utilizado por la estrategiaAlmacenamiento*/
  virtual void escribir(const void* bytes,size_t cantidad);
  virtual void escribir(const void* unByte);
  virtual void leer(void* bytes,size_t cantidad);
  virtual void leer(void* unBytes);
  virtual void posicionarByte(size_t posicion);
  virtual bool bienByte();
  /*------------------------------------------------------------------------*/
  //TODO usar un unico posicionamiento o usar uno para lectura y otro para escritura?
  virtual void posicionarCompuesto(size_t nroCompuesto);
  virtual void escribir(Compuesto*compuesto);
  virtual void leer(Compuesto*compuesto);
  virtual bool fin();
  virtual bool bien();
};

#endif // ARCHIVO_H
