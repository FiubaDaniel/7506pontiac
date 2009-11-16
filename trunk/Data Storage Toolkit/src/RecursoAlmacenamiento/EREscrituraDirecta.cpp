#include "EREscrituraDirecta.h"

EREscrituraDirecta::EREscrituraDirecta(EstrategiaIndice* indice,Almacenamiento*archivo,Almacenamiento*buffer,size_t cantidadElementoBuffer){
	this->archivo=archivo;
	this->buffer=buffer;
	this->indice=indice;
	this->archivo->getEstrategia()->setColaCambios(&this->cambiosArchivo);
	this->buffer->getEstrategia()->setColaCambios(NULL);
	admin.capacidad=cantidadElementoBuffer;
	componenteBuffer=buffer->getEstrategia()->getComponenteUsado()->clonar();
	clave=this->archivo->getEstrategia()->getClave();
}

EREscrituraDirecta::~EREscrituraDirecta(){
	delete componenteBuffer;
}

void EREscrituraDirecta::setClave(Registro*reg,Clave*clave){
	for(Ttamanio i=0;i<clave->getCantidadAtributos();i++){
		Atributo*att=clave->getAtributo(i);
		*reg->get(att->getNombre())=*att;
	}
}
bool EREscrituraDirecta::insertar(Registro* registro){
	clave->set(registro);
	Referencia referencia;
	if(indice!=NULL){
		if(indice->BuscarReferencia(clave,&referencia))
			return false;
	}else {
		if(buffer->buscar(registro))
			return false;
		if(archivo->buscar(registro))
			return false;
	}
	if(!archivo->insertar(registro))
		return false;
	while(!cambiosArchivo.empty()){
		Cambio& cambio=cambiosArchivo.front();
		actualizarBuffer(cambio);
		actualizarIndice(cambio);
		cambiosArchivo.pop();
	};
	return true;
}
bool EREscrituraDirecta::eliminar(Clave* unaClave){
	Referencia referencia;
	if(indice!=NULL){
		if(!indice->BuscarReferencia(unaClave,&referencia))
			return false;
		archivo->posicionarComponente(referencia);
	}else {
		if(not archivo->buscar(unaClave))
			return false;
	}
	if(not archivo->eliminar(unaClave))
		return false;
	while(!cambiosArchivo.empty()){
		Cambio& cambio=cambiosArchivo.front();
		actualizarBuffer(cambio);
		actualizarIndice(cambio);
		cambiosArchivo.pop();
	};
	return true;
}
bool EREscrituraDirecta::modificar(Clave* unaClave,Registro* registro){
	clave->set(registro);
	Referencia referencia;
	if(indice!=NULL){
		if(!indice->BuscarReferencia(clave,&referencia))
			return false;
		archivo->posicionarComponente(referencia);
	}else {
		if( not archivo->buscar(unaClave))
			return false;
	}
	if(!archivo->modificar(registro))
		return false;
	while(!cambiosArchivo.empty()){
		Cambio& cambio=cambiosArchivo.front();
		actualizarBuffer(cambio);
		actualizarIndice(cambio);
		cambiosArchivo.pop();
	};
	return true;
}


bool EREscrituraDirecta::obtener(Clave* unaClave,Registro*registro){
	Referencia referencia;
	Referencia refBuffer;
	if(indice!=NULL){
		if(!indice->BuscarReferencia(clave,&referencia))
			return false;
		if(admin.buscar(referencia,refBuffer)){
			buffer->posicionarComponente(refBuffer);
			buffer->leer(componenteBuffer);
			if(not ((ComponenteMemoria*)componenteBuffer)->estaSucio()){
				buffer->posicionarComponente(refBuffer);
				return buffer->obtener(registro);
			}
		}
		archivo->posicionarComponente(referencia);
		if(archivo->obtener(registro)){
			insertarEnBuffer(referencia);
			return true;
		}
	}else{
		if(buffer->buscar(registro))
			return true;
		if(archivo->buscar(registro)){
			referencia=archivo->posicionComponente();
			insertarEnBuffer(referencia);
			return true;
		}

	}
	return false;
}
void EREscrituraDirecta::insertarEnBuffer(Referencia refArchivo){
	Referencia refBuffer;
	if(!admin.buscar(refArchivo,refBuffer))
		admin.insertar(refArchivo,refBuffer);
	admin.imprimir(cout);
	Componente *componente=archivo->getEstrategia()->getComponenteUsado()->clonar();
	archivo->posicionarComponente(refArchivo);
	archivo->leer(componente);
	componenteBuffer->copiar(componente);
	ComponenteMemoria* compMem=dynamic_cast<ComponenteMemoria*>(componenteBuffer);
	compMem->setSucio(false);
	buffer->posicionarComponente(refBuffer);
	buffer->escribir(componenteBuffer);
	delete componente;
}
void EREscrituraDirecta::actualizarIndice(Cambio cambio){
	if(indice==NULL)
		return;
	switch(cambio.operacion){
	case Cambio::Alta : indice->insertar(cambio.referencia,&cambio.clave); break;
	case Cambio::Baja : indice->eliminar(&cambio.clave); break;
	case Cambio::Reubicacion : indice->modificar(&cambio.clave,cambio.referencia); break;
	default:break;
	}
}
void EREscrituraDirecta::actualizarBuffer(Cambio cambio){
	Referencia posicionBuffer;
	Componente* componente;
	ComponenteMemoria* compMem;
	switch(cambio.operacion){
	case Cambio::Alta :
		insertarEnBuffer(cambio.referencia);
		break;
	//case Cambio::Modificacion:
	case Cambio::Baja :
		if(admin.buscar(cambio.referencia,posicionBuffer)){
			buffer->posicionarComponente(posicionBuffer);
			buffer->leer(componenteBuffer);
			compMem=dynamic_cast<ComponenteMemoria*>(componenteBuffer);
			compMem->setSucio(true);
			buffer->posicionarComponente(posicionBuffer);
			buffer->escribir(componenteBuffer);
			//ponerlo como primero a ser reciclado
			admin.promoverAPrimero(cambio.referencia);
		}
		break;
	case Cambio::Reubicacion :
		Referencia posicionAnterior;
		indice->BuscarReferencia(&cambio.clave,&posicionAnterior);
		/*si el Componente de la posicion previa
		 * esta en el buffer lo marco como sucio*/
		if(admin.buscar(posicionAnterior,posicionBuffer)){
			//marcar componente como sucio
			buffer->posicionarComponente(posicionBuffer);
			buffer->leer(componenteBuffer);
			compMem=dynamic_cast<ComponenteMemoria*>(componenteBuffer);
			compMem->setSucio(true);
			buffer->posicionarComponente(posicionBuffer);
			buffer->escribir(componenteBuffer);
			//ponerlo como primero a ser usado
			admin.promoverAPrimero(posicionAnterior);
		}
		/*actualizo la nueva posicion si esta en el buffer*/
		if(!admin.buscar(cambio.referencia,posicionBuffer))
			admin.insertar(cambio.referencia,posicionBuffer);
		archivo->posicionarComponente(cambio.referencia);
		componente=archivo->getEstrategia()->getComponenteUsado()->clonar();
		archivo->leer(componente);
		componenteBuffer->copiar(componente);
		compMem=dynamic_cast<ComponenteMemoria*>(componenteBuffer);
		compMem->setSucio(false);
		buffer->posicionarComponente(posicionBuffer);
		buffer->escribir(componenteBuffer);
		delete componente;
		break;
	default:break;
	}

}
/*---------------------------------------------------------------------------------------------------------------*/

EstrategiaIndice *EREscrituraDirecta::getIndice(){
	return indice;
}

Almacenamiento *EREscrituraDirecta::getAlmacenamiento(){
	return archivo;
}
Almacenamiento *EREscrituraDirecta::getBuffer(){
	return buffer;
}


void EREscrituraDirecta::setIndice(EstrategiaIndice *indice){
	this->indice=indice;
}
void EREscrituraDirecta::cerrar(){
	indice->cerrar();
	buffer->cerrar();
	archivo->cerrar();
}






