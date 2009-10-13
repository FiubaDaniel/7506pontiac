
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
};

#endif // ARCHIVO_H
