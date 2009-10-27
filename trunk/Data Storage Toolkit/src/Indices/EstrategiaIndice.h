
#ifndef ESTRATEGIAINDICE_H
#define ESTRATEGIAINDICE_H

#include <string>

/**
  * class EstrategiaIndice
  * 
  */

/******************************* Abstract Class ****************************
EstrategiaIndice does not have any pure virtual methods, but its author
  defined it as an abstract class, so you should not use it directly.
  Inherit from it instead and create only objects from the derived classes
*****************************************************************************/

class EstrategiaIndice
{
public:

	void crear(char* nombreArch,char* nomEspaciosLibres,unsigned int tamanioBloque,int tamanioSerializadoClave,ComparadorClave* comp);
	void abrir(char* nombreArch,char* nomEspaciosLibres,ComparadorClave* comp);
	bool BuscarReferencia(Clave clave,Referencia* referencia);
	Referencia Siguiente(bool ultimo);
	bool insertar(Referencia ref,Clave* clave);
    bool eliminar(Clave* clave);
	bool modificar(Clave clave,Referencia refNueva);

protected:

public:


  // Protected attribute accessor methods
  //  

protected:

public:


  // Protected attribute accessor methods
  //  

protected:


private:

public:


  // Private attribute accessor methods
  //  

private:

public:


  // Private attribute accessor methods
  //  

private:



};

#endif // ESTRATEGIAINDICE_H
