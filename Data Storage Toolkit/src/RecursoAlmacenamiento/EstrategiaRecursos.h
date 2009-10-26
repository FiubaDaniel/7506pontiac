
#ifndef ESTRATEGIARECURSOS_H
#define ESTRATEGIARECURSOS_H
#include "RecursoAlmacenamiento.h"
#include "Componente.h"

class EstrategiaRecursos{
public:
	virtual boolean er_guardar();
	virtual boolean er_eliminar();
	virtual boolean er_modificar();
	virtual Componente er_obtener();
private:

};

#endif // ESTRAGIARECURSOS_H
