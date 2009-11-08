#include "ERUnAlmacenamiento.h"

ERUnAlmacenamiento::ERUnAlmacenamiento(EstrategiaIndice*indice,EstrategiaAlmacenamiento*estrategia){
	estrategiAlmacenamiento=estrategia;
	estrategiAlmacenamiento->logActivo=true;
	this->indice=indice;
	registro=(Registro*)estrategia->getRegistro();
	clave=estrategia->getClave();
}

ERUnAlmacenamiento::~ERUnAlmacenamiento(){}

bool ERUnAlmacenamiento::insertar(Registro* registro){
	clave->set(registro);
	Referencia referencia;
	if(indice!=NULL){
		if(indice->BuscarReferencia(clave,&referencia))
			return false;
	}else {
		estrategiAlmacenamiento->logActivo=false;
		if(estrategiAlmacenamiento->buscar(registro))
			return false;
	}
	if(!estrategiAlmacenamiento->insertar(registro))return false;
	while(!estrategiAlmacenamiento->NoHayMasCambios()){
		const Cambio* cambio=estrategiAlmacenamiento->siguienteCambio();
		actualizarIndice(*cambio);
		estrategiAlmacenamiento->pop();
	};
	return true;
}
bool ERUnAlmacenamiento::eliminar(Clave* unaClave){
	Referencia referencia;
	if(indice!=NULL){
		if(!indice->BuscarReferencia(unaClave,&referencia))
			return false;
	}else {
		estrategiAlmacenamiento->logActivo=true;
		setClave(registro,clave);
		if(!estrategiAlmacenamiento->buscar(registro))
			return false;
		referencia=estrategiAlmacenamiento->siguienteCambio()->referencia;
		estrategiAlmacenamiento->pop();
		estrategiAlmacenamiento->logActivo=false;
	}
	estrategiAlmacenamiento->posicionarComponente(referencia);
	setClave(registro,clave);
	if(!estrategiAlmacenamiento->eliminar(registro))return false;
	if(indice!=NULL)
		indice->eliminar(unaClave);
	while(!estrategiAlmacenamiento->NoHayMasCambios()){
		const Cambio* cambio=estrategiAlmacenamiento->siguienteCambio();
		actualizarIndice(*cambio);
		estrategiAlmacenamiento->pop();
	};
	return true;
}
bool ERUnAlmacenamiento::modificar(Clave* unaClave,Registro* registro){
	clave->set(registro);
	Referencia referencia;
	if(indice!=NULL){
		if(!indice->BuscarReferencia(unaClave,&referencia))
			return false;
	}else {
		estrategiAlmacenamiento->logActivo=true;
		setClave(registro,clave);
		if(!estrategiAlmacenamiento->buscar(registro))
			return false;
		referencia=estrategiAlmacenamiento->siguienteCambio()->referencia;
		estrategiAlmacenamiento->pop();
		estrategiAlmacenamiento->logActivo=false;
	}
	estrategiAlmacenamiento->posicionarComponente(referencia);
	if(!estrategiAlmacenamiento->modificar(registro))
		return false;
	while(!estrategiAlmacenamiento->NoHayMasCambios()){
		const Cambio* cambio=estrategiAlmacenamiento->siguienteCambio();
		actualizarIndice(*cambio);
		estrategiAlmacenamiento->pop();
	};
	return true;
}
bool ERUnAlmacenamiento::obtener(Clave* unaClave,Registro*registro){
	Referencia referencia;
	if(indice!=NULL){
		if(!indice->BuscarReferencia(clave,&referencia))
			return false;
		estrategiAlmacenamiento->posicionarComponente(referencia);
		return estrategiAlmacenamiento->obtener(registro);
	}else{
		estrategiAlmacenamiento->logActivo=false;
		setClave(registro,clave);
		return estrategiAlmacenamiento->buscar(registro);
	}
}

void ERUnAlmacenamiento::actualizarIndice(Cambio cambio){
	switch(cambio.operacion){
		case Cambio::Alta : indice->insertar(cambio.referencia,&cambio.clave); break;
		case Cambio::Baja : indice->eliminar(&cambio.clave); break;
		case Cambio::Reubicacion : indice->modificar(&cambio.clave,cambio.referencia); break;
		default:break;
	}
}
void ERUnAlmacenamiento::setClave(Registro*reg,Clave*clave){
	for(Ttamanio i=0;i<clave->getCantidadAtributos();i++){
		Atributo*att=clave->getAtributo(i);
		reg->get(att->getNombre())->set(att);
	}
}

EstrategiaAlmacenamiento *ERUnAlmacenamiento::getEstrategiaAlmacenamiento(){
    return estrategiAlmacenamiento;
}

EstrategiaIndice *ERUnAlmacenamiento::getIndice(){
    return indice;
}

void ERUnAlmacenamiento::setEstrategiAlmacenamiento(EstrategiaAlmacenamiento *estrategiAlmacenamiento)
{
    this->estrategiAlmacenamiento = estrategiAlmacenamiento;
    this->estrategiAlmacenamiento->logActivo=true;
    clave=estrategiAlmacenamiento->getClave();
    registro=(Registro*)estrategiAlmacenamiento->getRegistro();
}

void ERUnAlmacenamiento::setIndice(EstrategiaIndice *indice){
    this->indice = indice;
}

