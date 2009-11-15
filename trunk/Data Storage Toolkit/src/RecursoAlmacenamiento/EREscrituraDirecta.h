#ifndef ESTRATEGIARECURSOSESCRITURADIRECTA_H
#define ESTRATEGIARECURSOSESCRITURADIRECTA_H
#include "EstrategiaRecursos.h"
#include "../Compuesto/ComponenteMemoria.h"
#include <map>

/**
 * class EREscrituraDirecta
 *
 */
struct NodoArchivoBuffer;
typedef  std::map<size_t,NodoArchivoBuffer>::iterator PNodoSiguiente;
struct NodoArchivoBuffer{
	size_t posicionBuffer;
	PNodoSiguiente siguiente;
};

class AdministradorDeBuffer{
public:
	size_t capacidad;
	std::map<size_t,NodoArchivoBuffer> tablaArchivoBuffer;
	PNodoSiguiente primero;
	PNodoSiguiente ultimo;
	AdministradorDeBuffer(size_t capacidad){
		this->capacidad=capacidad;
		tablaArchivoBuffer.clear();
	}
	AdministradorDeBuffer(){
		this->capacidad=0;
		tablaArchivoBuffer.clear();
	}
	virtual ~AdministradorDeBuffer(){

	};
	bool buscar(size_t posicionArchivo,size_t&posicionBuffer){
		PNodoSiguiente it;
		it=tablaArchivoBuffer.find(posicionArchivo);
		posicionBuffer=it->second.posicionBuffer;
		return it!=tablaArchivoBuffer.end();
	};
	/*pre: it apunta a una elemento de la lista*/
	bool promoverAUltimo(PNodoSiguiente it){
		/*si it es el ultimo no hay nada q hacer */
		if(it==ultimo)

			return true;

		if(it==primero){

			primero=it->second.siguiente;

		}else{
			/*busco el anterior a it */
			PNodoSiguiente anterior=primero;

			while(anterior->second.siguiente!=it and anterior->second.siguiente!=tablaArchivoBuffer.end()){
				anterior=anterior->second.siguiente;
			}
			/*si no encontro sale*/
			if(anterior->second.siguiente==tablaArchivoBuffer.end())
				return false;
			/*saco al it de la cola*/
			anterior->second.siguiente=it->second.siguiente;
		}
		/*coloco it al final*/
		it->second.siguiente=ultimo->second.siguiente;
		ultimo->second.siguiente=it;
		ultimo=it;
		return true;
	};

	bool promoverAUltimo(size_t posicionArchivo){
		PNodoSiguiente it;
		it=tablaArchivoBuffer.find(posicionArchivo);
		if(it!=tablaArchivoBuffer.end())
			return promoverAUltimo(it);
		return false;
	}
	void insertar(size_t posicionArchivo,size_t&posicioBuffer){
		PNodoSiguiente it;
		NodoArchivoBuffer nuevo;
		/*Si la tabla esta llena*/
		if(tablaArchivoBuffer.size()>=capacidad){
			//saco el primero
			it=primero;
			primero=it->second.siguiente;
			//conservo la informacion sobre posicion buffer
			nuevo=it->second;
			tablaArchivoBuffer.erase(it);
		}else{
			nuevo.posicionBuffer=tablaArchivoBuffer.size();
		}
		/*inserto la nueva posicionArchivo*/
		pair<PNodoSiguiente,bool> resultado=tablaArchivoBuffer.insert(std::pair<size_t,NodoArchivoBuffer> (posicionArchivo,nuevo));
		if(!resultado.second)
			return;
		it=resultado.first;
		// si es el primero
		if(tablaArchivoBuffer.size()==1){
			ultimo=it;
			ultimo->second.siguiente=tablaArchivoBuffer.end();
			primero=it;
		}else{
			/*lo agrego al final de la cola*/
			it->second.siguiente=ultimo->second.siguiente;
			ultimo->second.siguiente=it;
			ultimo=it;
		}
		posicioBuffer=ultimo->second.posicionBuffer;
	};
	bool promoverAPrimero(PNodoSiguiente it){
		/*si it es el primero no hay nada q hacer */
		if(it!=primero){
			/*busco el anterior a it */
			PNodoSiguiente anterior=primero;

			while(anterior->second.siguiente!=it and anterior->second.siguiente!=tablaArchivoBuffer.end()){
				anterior=anterior->second.siguiente;
			}
			/*si no encontro sale*/
			if(anterior->second.siguiente==tablaArchivoBuffer.end())
				return false;
			/*saco al it de la cola*/
			anterior->second.siguiente=it->second.siguiente;
			if(it==ultimo)
				ultimo=anterior;
		}
		/*coloco it al principio*/

		it->second.siguiente=primero;
		primero=it;
		return true;
	};
	bool promoverAPrimero(size_t posicionArchivo){
		PNodoSiguiente it;
		it=tablaArchivoBuffer.find(posicionArchivo);
		if(it!=tablaArchivoBuffer.end())
			return promoverAPrimero(it);
		return false;
	}
	void imprimir(std::ostream&out){
		PNodoSiguiente siguiente=primero;
		while(siguiente!=tablaArchivoBuffer.end()){
			out<<"PosArchivo:"<<siguiente->first;
			out<<" PosBuffer:"<<siguiente->second.posicionBuffer;
			out<<" SiguientePosArchivo:"<<siguiente->second.siguiente->first<<endl;
			siguiente=siguiente->second.siguiente;
		}
	}
};

class EREscrituraDirecta : public EstrategiaRecursos{
private:
	AdministradorDeBuffer admin;
	std::queue<Cambio> cambiosArchivo;
	Almacenamiento*archivo;
	Almacenamiento*buffer;
	EstrategiaIndice* indice;
	Clave*clave;
	Componente*componenteBuffer;
	void actualizarIndice(Cambio cambio);
	void actualizarBuffer(Cambio cambio);
	void insertarEnBuffer(Referencia refArchivo);
	void setClave(Registro*reg,Clave*clave);
public:
	EREscrituraDirecta(EstrategiaIndice* indice,Almacenamiento*archivo,Almacenamiento*buffer,size_t cantidadElementoBuffer);
	virtual ~EREscrituraDirecta();
	bool insertar(Registro* registro);
	bool eliminar(Clave* unaClave);
	bool modificar(Clave* unaClave,Registro* registro);
	bool obtener(Clave* unaClave,Registro*registro);
	void cerrar();
	Almacenamiento *getAlmacenamiento();
	Almacenamiento *getBuffer();
	EstrategiaIndice *getIndice();
	void setIndice(EstrategiaIndice *indice);

};

#endif // ESTRATEGIARECURSOSESCRITURADIRECTA_H
