#ifndef ESTRATEGIAALMACENAMIENTO_H_
#define ESTRATEGIAALMACENAMIENTO_H_
#include <queue>
#include "Almacenamiento.h"
#include "../ArbolBSharp/ComparadorClave.h"
#include "../ArbolBSharp/Clave.h"

class Almacenamiento;
class Cambio{
public:
	Clave clave;
	Referencia referencia;
	enum TOperacion{Alta,Baja,Reubicacion,Modificacion} operacion;
public:
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
private:
	std::queue<Cambio*> cambiosLog;
protected:
	void pushCambio(const Cambio& cambio){
		cambiosLog.push(new Cambio(cambio));
	};
	Clave *clave;
	ComparadorClave*comparador;
public:
	EstrategiaAlmacenamiento(){clave=NULL;};
	virtual ~EstrategiaAlmacenamiento(){
		if(clave)
			delete clave;
		while(!cambiosLog.empty()){
			delete cambiosLog.front();
			cambiosLog.pop();
		}
	};
	virtual Componente* getComponente()=0;
	virtual void setClave(Clave*unaClave){clave=unaClave->clonarce();};
	virtual void setComparador(ComparadorClave*unComparador){
		comparador=unComparador;
	};
	virtual Almacenamiento* abrir(Almacenamiento*almacen)=0;
	virtual Almacenamiento* crear(Almacenamiento*almacen)=0;
	virtual bool posicionarComponente(size_t nroComponente)=0;
	virtual size_t posicionComponente()=0;
	virtual bool escribir(Componente*componente)=0;
	virtual bool leer(Componente*componente)=0;
	virtual bool insertar(Componente*componente)=0;
	virtual bool modificar(Componente*componente)=0;
	virtual bool eliminar(Componente*componente)=0;
	virtual bool siguiente(Componente*componente)=0;
	virtual bool obtener(Componente*componente)=0;
	bool logActivo;
	/* Cola que refleja los cambios producidos en la estructura del archivo
	 * a causa de las operaciones. La cola es exclusivamente para uso externo.
	 */
	void pop(){
		delete cambiosLog.front();
		cambiosLog.pop();
	};
	const Cambio* siguienteCambio(){
		return cambiosLog.front();
	};
	bool hayMasCambios(){
		return cambiosLog.empty();
	}
};
#endif /* ESTRATEGIAALMACENAMIENTO_H_ */
