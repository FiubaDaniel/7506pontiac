
#ifndef ESTRATEGIARECURSOSUNALMACENAMIENTO_H
#define ESTRATEGIARECURSOSUNALMACENAMIENTO_H
#include "EstrategiaRecursos.h"

#include <string>

class ERUnAlmacenamiento : public EstrategiaRecursos{
public:
	ERUnAlmacenamiento (EstrategiaIndice*indice,Almacenamiento *almacenamiento);
	virtual ~ERUnAlmacenamiento ( );
	bool insertar(Registro* registro);
	bool eliminar(Clave* unaClave);
	bool modificar(Clave* unaClave,Registro* registro);
	bool obtener(Clave* unaClave,Registro*registro);
	void cerrar();
    Almacenamiento *getAlmacenamiento();
    EstrategiaIndice *getIndice() ;
    void setIndice(EstrategiaIndice *indice);
private:
	void actualizarIndice(Cambio cambio);
	std::queue<Cambio> colaCambios;
	Almacenamiento* almacen;
	EstrategiaIndice* indice;
	Clave*clave;
};

#endif // ESTRATEGIARECURSOSUNALMACENAMIENTO_H
