#ifndef ESTRATEGIAALMACENAMIENTO_H_
#define ESTRATEGIAALMACENAMIENTO_H_
#include <queue>
#include "Almacenamiento.h"
#include "../ArbolBSharp/ComparadorClave.h"
#include "../ArbolBSharp/Clave.h"

class Almacenamiento;
class Cambio;

class EstrategiaAlmacenamiento {
protected:
	Clave*clave;
	ComparadorClave*comparador;
public:
	EstrategiaAlmacenamiento(){};
	virtual ~EstrategiaAlmacenamiento(){};
	virtual void setClave(Clave*unaClave){
		clave=unaClave;
	};
	virtual void setComparador(ComparadorClave*unComparador){
		comparador=unComparador;
	};
	virtual void posicionarComponente(size_t nroCompuesto)=0;
	virtual bool escribir(Componente*componente)=0;
	virtual bool leer(Componente*componente)=0;
	virtual size_t insertar(Componente*componente)=0;
	virtual bool modificar(Componente*componente)=0;
	virtual bool eliminar(Componente*componente)=0;
	virtual bool siguiente(Componente*componente)=0;
	bool logActivo;
	/* Cola que refleja los cambios producidos en la estructura del archivo
	 * a causa de las operaciones. La cola es exclusivamente para uso externo.
	 */
	std::queue<Cambio> cambiosLog;
};

class Cambio{
public:
	Clave *clave;
	Referencia referencia;
	enum TOperacion{Alta,Baja,Reubicacion} operacion;
public:
	Cambio(Clave*clave,Referencia referencia,TOperacion op){
		//TODO this->clave=clave->clonarce();
		this->referencia=referencia;
		this->operacion=op;
	}
	virtual ~Cambio(){
		delete clave;
	};
};
#endif /* ESTRATEGIAALMACENAMIENTO_H_ */
