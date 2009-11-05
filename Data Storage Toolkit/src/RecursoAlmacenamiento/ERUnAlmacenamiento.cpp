#include "ERUnAlmacenamiento.h"

ERUnAlmacenamiento::ERUnAlmacenamiento(){
}

ERUnAlmacenamiento::~ERUnAlmacenamiento(){}

bool ERUnAlmacenamiento::insertar(Registro* registro){
	clave->set(registro);
	Referencia referencia;
	if(indice->BuscarReferencia(clave,&referencia))
		return false;
	estrategiAlmacenamiento->insertar(registro);
	while(!estrategiAlmacenamiento->hayMasCambios()){
		const Cambio* cambio=estrategiAlmacenamiento->siguienteCambio();
		actualizarIndice(*cambio);
		estrategiAlmacenamiento->pop();
	};
	return true;
};
bool ERUnAlmacenamiento::eliminar(Clave* unaClave){
	Referencia referencia;
	if(!indice->BuscarReferencia(unaClave,&referencia))
		return false;
	estrategiAlmacenamiento->posicionarComponente(referencia);
	setClave(registro,clave);
	estrategiAlmacenamiento->eliminar(registro);
	indice->eliminar(unaClave);
	while(!estrategiAlmacenamiento->hayMasCambios()){
		const Cambio* cambio=estrategiAlmacenamiento->siguienteCambio();
		actualizarIndice(*cambio);
		estrategiAlmacenamiento->pop();
	};
	return true;
};
bool ERUnAlmacenamiento::modificar(Clave* unaClave,Registro* registro){
	clave->set(registro);
	Referencia referencia;
	if(!indice->BuscarReferencia(unaClave,&referencia))
		return false;
	estrategiAlmacenamiento->posicionarComponente(referencia);
	if(!estrategiAlmacenamiento->modificar(registro))
		return false;
	while(!estrategiAlmacenamiento->hayMasCambios()){
		const Cambio* cambio=estrategiAlmacenamiento->siguienteCambio();
		actualizarIndice(*cambio);
		estrategiAlmacenamiento->pop();
	};
	return true;
};
bool ERUnAlmacenamiento::obtener(Clave* unaClave,Registro*registro){
	Referencia referencia;
	if(!indice->BuscarReferencia(clave,&referencia))
		return false;
	estrategiAlmacenamiento->posicionarComponente(referencia);
	return estrategiAlmacenamiento->obtener(registro);
};

void ERUnAlmacenamiento::actualizarIndice(Cambio cambio){
	switch(cambio.operacion){
		case Cambio::Alta : indice->insertar(cambio.referencia,&cambio.clave); break;
		case Cambio::Baja : indice->eliminar(&cambio.clave); break;
		case Cambio::Reubicacion : indice->modificar(&cambio.clave,cambio.referencia); break;
	}
}
void ERUnAlmacenamiento::setClave(Registro*reg,Clave*clave){
	for(Ttamanio i=0;i<clave->getCantidadAtributos();i++){
		Atributo*att=clave->getAtributo(i);
		reg->get(att->getNombre())->set(att);
	}
};


