#ifndef ESTRATEGIAALMACENAMIENTO_H_
#define ESTRATEGIAALMACENAMIENTO_H_
#include <queue>
#include "../ArbolBSharp/ComparadorClave.h"
#include "../ArbolBSharp/Clave.h"

class Almacenamiento;

class Cambio{
public:
	Clave clave;
	Referencia referencia;
	enum TOperacion{Alta,Baja,Reubicacion,Modificacion} operacion;
public:
	Cambio(const Cambio&cambio):clave(cambio.clave){
			referencia=cambio.referencia;
			operacion=cambio.operacion;
	};
	Cambio(const Cambio*cambio):clave(cambio->clave){
		referencia=cambio->referencia;
		operacion=cambio->operacion;
	};
	Cambio(Clave* unaClave,Referencia referencia,TOperacion op):clave(*unaClave){
		this->referencia=referencia;
		this->operacion=op;
	}
	~Cambio(){}
};

class EstrategiaAlmacenamiento {
protected:
	std::queue<Cambio>* colaDeCambios;
	Clave *clave;
	ComparadorClave*comparador;
	Almacenamiento*almacen;
public:
	EstrategiaAlmacenamiento(){
		almacen=NULL;
		colaDeCambios=NULL;
		clave=NULL;
	};
	virtual ~EstrategiaAlmacenamiento(){
		if(clave)
			delete clave;
	};
	void setAlmacenamiento(Almacenamiento*almacen){
		if(this->almacen){
			cerrar();
		}
		this->almacen=almacen;
	}
	/*******************************************************************************/
	virtual bool abrir()=0;
	virtual void crear()=0;
	virtual void cerrar()=0;
	virtual bool posicionarComponente(Referencia nroComponente)=0;
	virtual Referencia posicionComponente()=0;
	virtual bool escribir(Componente*componente)=0;
	virtual bool leer(Componente*componente)=0;
	virtual bool insertar(Componente*componente)=0;
	virtual bool modificar(Componente*componente)=0;
	virtual bool eliminar(Componente*componente)=0;
	virtual bool eliminar(Clave*clave)=0;
	virtual bool siguiente(Componente*componente)=0;
	virtual bool obtener(Componente*componente)=0;
	virtual bool buscar(Componente*componente)=0;
	virtual bool buscar(Clave*unaClave)=0;
	/*******************************************************************************/
	void setClave(Clave*unaClave){
		delete clave;
		clave=unaClave->clonarce();
	};
	Clave* getClave(){
		return clave;
	}
	void setComparador(ComparadorClave*unComparador){
		comparador=unComparador;
	};
	ComparadorClave* getComparador(){
		return comparador;
	};
	void setColaCambios(std::queue<Cambio>* cambiosLog){
		this->colaDeCambios=cambiosLog;
	}
	virtual Componente*getComponenteUsado()=0;
	virtual void imprimirMetada(std::ostream&out)=0;
};
#endif /* ESTRATEGIAALMACENAMIENTO_H_ */
