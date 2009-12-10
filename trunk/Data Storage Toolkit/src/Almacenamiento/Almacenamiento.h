
#ifndef ALMACENAMIENTO_H
#define ALMACENAMIENTO_H
#include <string>
#include "../Compuesto/Compuesto.h"
#include "EstrategiaAlmacenamiento.h"
/**
 * class Almacenamiento
 *
 */
typedef int Referencia;
class EstrategiaAlmacenamiento;

class Almacenamiento{
protected:
	std::string nombre;
	EstrategiaAlmacenamiento*estrategia;
public:
	Almacenamiento (){};
	virtual ~Almacenamiento (){};
	std::string getNombre(){
		return nombre;
	}
	/*-------------------------------------------------------------------------*/
	/*utilizado por la estrategiaAlmacenamiento*/
	virtual bool abrir(const char* nombre)=0;
	virtual void crear(const char *nombre)=0;
	virtual void escribir(const void* bytes,Referencia cantidad)=0;
	virtual void escribir(const void* unByte)=0;
	virtual void leer(void* bytes,Referencia cantidad)=0;
	virtual void leer(void* unBytes)=0;
	virtual void posicionar(Referencia posicion)=0;
	virtual void posicionarAlfinal()=0;
	virtual Referencia posicionActual()=0;
	virtual bool bien()=0;
	virtual bool fin()=0;
	/*Limpia los flags de fin de archivo y error*/
	virtual void clear()=0;
	virtual void cerrar()=0;
	virtual Almacenamiento*clonar()=0;

	EstrategiaAlmacenamiento*getEstrategia(){
		return estrategia;
	}
	void setEstrategia(EstrategiaAlmacenamiento*estrategia){
		this->estrategia=estrategia;
	}
	/*		*/
	virtual bool posicionarComponente(Referencia nroComponente){
		if(estrategia)
			return estrategia->posicionarComponente(nroComponente);
		return false;
	};
	virtual Referencia ultimoComponente(){
		if(estrategia)
			return  estrategia->ultimoComponente();
		return 0;
	};
	virtual bool truncar(Referencia ultimo){
		if(estrategia)
					return  estrategia->truncar(ultimo);
		return false;
	}
	virtual Referencia posicionComponente(){
		if(estrategia)
			return estrategia->posicionComponente();
		return false;
	};
	virtual bool escribir(Componente*componente){
		if(estrategia)
			return estrategia->escribir(componente);
		return false;
	};
	virtual bool leer(Componente*componente){
		if(estrategia)
			return estrategia->leer(componente);
		return false;
	};
	virtual bool insertar(Componente*componente){
		if(estrategia)
			return estrategia->insertar(componente);
		return false;
	};
	virtual bool modificar(Componente*componente){
		if(estrategia)
			return estrategia->modificar(componente);
		return false;
	};
	virtual bool eliminar(Componente*componente){
		if(estrategia)
			return estrategia->eliminar(componente);
		return false;
	};
	virtual bool eliminar(Clave*clave){
		if(estrategia)
			return estrategia->eliminar(clave);
		return false;
	};
	virtual bool siguiente(Componente*componente){
		if(estrategia)
			return estrategia->siguiente(componente);
		return false;
	};
	virtual bool obtener(Componente*componente){
		if(estrategia)
			return estrategia->obtener(componente);
		return false;
	};
	virtual bool buscar(Componente*componente){
		if(estrategia)
			return estrategia->buscar(componente);
		return false;
	};
	virtual bool buscar(Clave*unaClave){
		if(estrategia)
			return estrategia->buscar(unaClave);
		return false;
	};

	virtual void imprimir(ostream& out){
		if(estrategia){
			estrategia->imprimirMetada(out);
			Componente*componente=estrategia->getComponenteUsado()->clonar();
			posicionarComponente(0);
			while(leer(componente)){
				out<<posicionComponente()-1 <<" : ";
				componente->imprimir(out);
				out<<endl;
			};
			delete componente;
		}
	}
};

#endif // ALMACENAMIENTO_H

