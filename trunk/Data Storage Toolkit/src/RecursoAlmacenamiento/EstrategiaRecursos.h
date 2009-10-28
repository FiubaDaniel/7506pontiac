#ifndef ESTRATEGIARECURSOS_H
#define ESTRATEGIARECURSOS_H
#include "Recurso.h"
#include "../Compuesto/Registro.h"
#include "../ArbolB#/Clave.h"
class Recurso;

class EstrategiaRecursos{
public:
	   virtual bool insertar(Registro* registro)=0;
	   virtual bool eliminar(Clave* unaClave)=0;
	   virtual bool modificar(Clave* unaClave,Registro* registro)=0;
	   virtual bool obtener(Clave* unaClave,Registro*registro)=0;
};
#endif // ESTRAGIARECURSOS_H
