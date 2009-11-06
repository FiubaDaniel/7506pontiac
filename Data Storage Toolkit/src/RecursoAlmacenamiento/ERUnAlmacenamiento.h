
#ifndef ESTRATEGIARECURSOSUNALMACENAMIENTO_H
#define ESTRATEGIARECURSOSUNALMACENAMIENTO_H
#include "EstrategiaRecursos.h"

#include <string>

class ERUnAlmacenamiento : public EstrategiaRecursos{
public:
	ERUnAlmacenamiento ( );
	virtual ~ERUnAlmacenamiento ( );
	bool insertar(Registro* registro);
	bool eliminar(Clave* unaClave);
	bool modificar(Clave* unaClave,Registro* registro);
	bool obtener(Clave* unaClave,Registro*registro);
	void setClave(Registro*reg,Clave*clave);
    Clave *getClave() const;
    EstrategiaAlmacenamiento *getEstrategiAlmacenamiento() const;
    EstrategiaIndice *getIndice() const;
    Registro *getRegistro() const;
    void setEstrategiAlmacenamiento(EstrategiaAlmacenamiento *estrategiAlmacenamiento);
    void setIndice(EstrategiaIndice *indice);
    void setRegistro(Registro *registro);
private:
	void actualizarIndice(Cambio cambio);
	EstrategiaAlmacenamiento* estrategiAlmacenamiento;
	EstrategiaIndice* indice;
	Registro*registro;
	Clave*clave;
};

#endif // ESTRATEGIARECURSOSUNALMACENAMIENTO_H
