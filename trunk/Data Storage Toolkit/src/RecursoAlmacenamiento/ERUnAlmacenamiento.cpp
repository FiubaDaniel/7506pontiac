#include "ERUnAlmacenamiento.h"

ERUnAlmacenamiento::ERUnAlmacenamiento ( ) {
}

ERUnAlmacenamiento::~ERUnAlmacenamiento ( ) { }

bool ERUnAlmacenamiento::insertar(Registro* registro){
	Clave*clave;
	EstrategiaAlmacenamiento* estrategiAlmacenamiento;
	EstrategiaIndice* indice;
	/**/
	clave->set(registro);
	Referencia referencia;
	if(indice->BuscarReferencia(*clave,&referencia))
		return false;
	referencia=estrategiAlmacenamiento->insertar(registro);
	while(!estrategiAlmacenamiento->cambiosLog.empty()){
		Cambio cambio=estrategiAlmacenamiento->cambiosLog.front();
		actualizarIndice(cambio);
		estrategiAlmacenamiento->cambiosLog.pop();
	};
	return true;
};
bool ERUnAlmacenamiento::eliminar(Clave* unaClave){
	EstrategiaAlmacenamiento* estrategiAlmacenamiento=NULL;
	EstrategiaIndice* indice=NULL;
	Registro*registro=NULL;
	/**/
	//TODO registro.set(clave);

	Referencia referencia;
	if(indice->BuscarReferencia(*unaClave,&referencia))
		return false;
	estrategiAlmacenamiento->eliminar(registro);
	indice->eliminar(unaClave);
	while(!estrategiAlmacenamiento->cambiosLog.empty()){
		Cambio cambio=estrategiAlmacenamiento->cambiosLog.front();
		actualizarIndice(cambio);
		estrategiAlmacenamiento->cambiosLog.pop();
	};
	return true;
};
bool ERUnAlmacenamiento::modificar(Clave* unaClave,Registro* registro){
	Clave*clave=NULL;
	EstrategiaAlmacenamiento* estrategiAlmacenamiento=NULL;
	EstrategiaIndice* indice=NULL;
	/**/
	clave->set(registro);
	Referencia referencia;
	if(!indice->BuscarReferencia(*clave,&referencia))
		return false;
	estrategiAlmacenamiento->posicionarComponente(referencia);
	if(!estrategiAlmacenamiento->modificar(registro))
		return false;
	while(!estrategiAlmacenamiento->cambiosLog.empty()){
		Cambio cambio=estrategiAlmacenamiento->cambiosLog.front();
		actualizarIndice(cambio);
		estrategiAlmacenamiento->cambiosLog.pop();
	};
	return true;
};
bool ERUnAlmacenamiento::obtener(Clave* unaClave,Registro*registro){
	Clave*clave=NULL;
	EstrategiaAlmacenamiento* estrategiAlmacenamiento=NULL;
	EstrategiaIndice* indice=NULL;
	/**/
	Referencia referencia;
	if(!indice->BuscarReferencia(*clave,&referencia))
		return false;
	estrategiAlmacenamiento->posicionarComponente(referencia);
	return estrategiAlmacenamiento->obtenerSiguiente(registro);
};

void ERUnAlmacenamiento::actualizarIndice(Cambio cambio){}



