
#ifndef ALMACENAMIENTO_H
#define ALMACENAMIENTO_H
#include "EstrategiaAlamcenamiento.h"
class EstrategiaAlamcenamiento;

/**
  * class Almacenamiento
  * 
  */

class Almacenamiento{
	EstrategiaAlamcenamiento* estrategiaUsada;
public:
  Almacenamiento ( );
  virtual ~Almacenamiento ( );
};

#endif // ALMACENAMIENTO_H
