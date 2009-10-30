
#ifndef ESTRATEGIAINDICE_H
#define ESTRATEGIAINDICE_H

#include <string>
#include "../ArbolBSharp/Clave.h"
#include "../ArbolBSharp/ComparadorClave.h"
/**
  * class EstrategiaIndice
  * 
  */

/******************************* Abstract Class ****************************
EstrategiaIndice does not have any pure virtual methods, but its author
  defined it as an abstract class, so you should not use it directly.
  Inherit from it instead and create only objects from the derived classes
*****************************************************************************/
/*TODO prueba commit*/
class EstrategiaIndice
{
public:
	virtual void crear(char* nombreArch,char* nomEspaciosLibres,unsigned int tamanioBloque,int tamanioSerializadoClave,ComparadorClave* comp)=0;
	virtual void abrir(char* nombreArch,char* nomEspaciosLibres,ComparadorClave* comp)=0;
	virtual bool BuscarReferencia(Clave clave,Referencia* referencia)=0;
	virtual Referencia Siguiente(bool ultimo)=0;
	virtual bool insertar(Referencia ref,Clave* clave)=0;
    virtual bool eliminar(Clave* clave)=0;
	virtual bool modificar(Clave clave,Referencia refNueva)=0;
};

#endif // ESTRATEGIAINDICE_H
