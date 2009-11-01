
#ifndef ARCHIVO_H
#define ARCHIVO_H
#include "Almacenamiento.h"
#include <fstream>
#include <string>

class Archivo : public Almacenamiento{
 std::fstream archivo;
public:
  Archivo ();
  virtual ~Archivo ();
  /*-------------------------------------------------------------------------*/
  /*utilizado por la estrategiaAlmacenamiento*/
  virtual void crear(const char*ruta);
  virtual void abrir(const char*ruta);
  virtual void cerrar();
  virtual void escribir(const void* bytes,size_t cantidad);
  virtual void escribir(const void* unByte);
  virtual void leer(void* bytes,size_t cantidad);
  virtual void leer(void* unBytes);
  virtual void posicionar(size_t posicion);
  virtual bool bien();
  virtual bool fin();
  virtual void posicionarAlfinal();
  virtual size_t posicionActual();
};

#endif // ARCHIVO_H
