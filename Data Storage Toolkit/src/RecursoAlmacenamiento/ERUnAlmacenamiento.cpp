#include "ERUnAlmacenamiento.h"

ERUnAlmacenamiento::ERUnAlmacenamiento(EstrategiaIndice*indice,Almacenamiento*archivo){
	almacen=archivo;
	almacen->getEstrategia()->setColaCambios(&colaCambios);
	this->indice=indice;
	clave=almacen->getEstrategia()->getClave()->clonarce();
}

ERUnAlmacenamiento::~ERUnAlmacenamiento(){
	delete clave;
}

bool ERUnAlmacenamiento::insertar(Registro* registro){
	clave->set(registro);
	Referencia referencia;
	if(indice!=NULL){
		if(indice->BuscarReferencia(clave,&referencia))
			return false;
	}else {
		if(almacen->buscar(registro))
			return false;
	}
	if(!almacen->insertar(registro))return false;
	while(!colaCambios.empty()){
		Cambio& cambio=colaCambios.front();
		actualizarIndice(cambio);
		colaCambios.pop();
	};
	return true;
}
bool ERUnAlmacenamiento::eliminar(Clave* unaClave){
	Referencia referencia;
	if(indice!=NULL){
		if(!indice->BuscarReferencia(unaClave,&referencia))
			return false;
	}else {
		if(!almacen->buscar(unaClave))
			return false;
		referencia=almacen->posicionComponente();
	}
	almacen->posicionarComponente(referencia);
	if(!almacen->eliminar(unaClave))
		return false;
	while(!colaCambios.empty()){
		Cambio& cambio=colaCambios.front();
		actualizarIndice(cambio);
		colaCambios.pop();
	};
	return true;
}
bool ERUnAlmacenamiento::modificar(Clave* unaClave,Registro* registro){
	Referencia referencia;
	if(indice!=NULL){
		if(!indice->BuscarReferencia(unaClave,&referencia))
			return false;
	}else {
		if(!almacen->buscar(unaClave))
			return false;
		referencia=almacen->posicionComponente();
	}
	almacen->posicionarComponente(referencia);
	if(!almacen->modificar(registro))
		return false;
	while(!colaCambios.empty()){
		Cambio& cambio=colaCambios.front();
		actualizarIndice(cambio);
		colaCambios.pop();
	};
	return true;
}
bool ERUnAlmacenamiento::obtener(Clave* unaClave,Registro*registro){
	Referencia referencia;
	if(indice!=NULL){
		if(!indice->BuscarReferencia(unaClave,&referencia))
			return false;

	}else{
		if(almacen->buscar(unaClave)){
			referencia=almacen->posicionComponente();
		}
	}
	almacen->posicionarComponente(referencia);
	return almacen->obtener(registro);
}

void ERUnAlmacenamiento::actualizarIndice(Cambio cambio){
	switch(cambio.operacion){
		case Cambio::Alta : indice->insertar(cambio.referencia,&cambio.clave); break;
		case Cambio::Baja : indice->eliminar(&cambio.clave); break;
		case Cambio::Reubicacion : indice->modificar(&cambio.clave,cambio.referencia); break;
		default:break;
	}
}

Almacenamiento *ERUnAlmacenamiento::getAlmacenamiento(){
    return almacen;
}

EstrategiaIndice *ERUnAlmacenamiento::getIndice(){
    return indice;
}

void ERUnAlmacenamiento::setIndice(EstrategiaIndice *indice){
    this->indice = indice;
}
void ERUnAlmacenamiento::cerrar(){
	indice->cerrar();
	almacen->cerrar();
}
