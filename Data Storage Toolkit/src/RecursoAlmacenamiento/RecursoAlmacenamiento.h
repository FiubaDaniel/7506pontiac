
#ifndef RECURSOALMACENAMIENTO_H
#define RECURSOALMACENAMIENTO_H
#include "EstrategiaRecursos.h"
class EstrategiaRecursos;
/**
  * class RecursoAlmacenamiento
  * 
  */
class RecursoAlmacenamiento{
	EstrategiaRecursos* estrategiaUsada;
public:

  RecursoAlmacenamiento ();
  virtual ~RecursoAlmacenamiento ( );
};

#endif // RECURSOALMACENAMIENTO_H
