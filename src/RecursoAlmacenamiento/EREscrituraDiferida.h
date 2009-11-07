
#ifndef ESTRATEGIARECURSOSESCRITURADIFERIDA_H
#define ESTRATEGIARECURSOSESCRITURADIFERIDA_H
#include "EstrategiaRecursos.h"

#include <string>

class EREscrituraDiferida : public EstrategiaRecursos{

public:
  EREscrituraDiferida ( );
  virtual ~EREscrituraDiferida ( );
  virtual bool insertar(Registro* registro)=0;
  virtual bool eliminar(Clave* unaClave)=0;
  virtual bool modificar(Clave* unaClave,Registro* registro)=0;
  virtual bool obtener(Clave* unaClave,Registro*registro)=0;
};

#endif // ESTRATEGIARECURSOSESCRITURADIFERIDA_H
