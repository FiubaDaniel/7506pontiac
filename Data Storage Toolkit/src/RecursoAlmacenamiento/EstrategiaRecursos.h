#ifndef ESTRATEGIARECURSOS_H
#define ESTRATEGIARECURSOS_H
#include "Recurso.h"
#include "../Compuesto/Registro.h"
class Recurso;
//Todo borrar
class Clave;
class Comparador;
//todo fin borrar
class EstrategiaRecursos{
public:
	   virtual bool agregar(Registro* registro)=0;
	   virtual bool eliminar(Clave unaClave)=0;
	   virtual bool modificar(Clave unaClave,Registro* registro)=0;
	   virtual bool obtener(Clave unaClave,Registro*registro)=0;
};
#endif // ESTRAGIARECURSOS_H
