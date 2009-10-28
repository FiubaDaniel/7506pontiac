
#ifndef ESTRATEGIARECURSOSUNALMACENAMIENTO_H
#define ESTRATEGIARECURSOSUNALMACENAMIENTO_H
#include "EstrategiaRecursos.h"

#include <string>

/**
  * class ERUnAlmacenamiento
  * 
  */

class ERUnAlmacenamiento : public EstrategiaRecursos
{
public:
  ERUnAlmacenamiento ( );
  virtual ~ERUnAlmacenamiento ( );
  virtual bool insertar(Registro* registro)=0;
  virtual bool eliminar(Clave* unaClave)=0;
  virtual bool modificar(Clave* unaClave,Registro* registro)=0;
  virtual bool obtener(Clave* unaClave,Registro*registro)=0;
private:

};

#endif // ESTRATEGIARECURSOSUNALMACENAMIENTO_H
