#include "EREscrituraDirecta.h"

EREscrituraDirecta::EREscrituraDirecta(EstrategiaIndice* indice,Almacenamiento*archivo,Almacenamiento*buffer,size_t cantidadElementoBuffer){
	this->archivo=archivo;
	this->buffer=buffer;
	this->indice=indice;
	this->archivo->getEstrategia()->setColaCambios(&this->cambiosArchivo);
	this->buffer->getEstrategia()->setColaCambios(NULL);
	admin.setCapacidad(cantidadElementoBuffer);
	//TODO registro=(Registro*)this->estrategiaArchivo->getRegistro()->clonar();
	clave=this->archivo->getEstrategia()->getClave();
}

EREscrituraDirecta::~EREscrituraDirecta(){

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
	}else {
		/*TODO usar cola de cambios*/
		if(archivo->buscar(unaClave)){
			//TODO ver q se borra del buffer
			archivo->eliminar(unaClave);
		}else return false;
	}
	archivo->posicionarComponente(referencia);
	archivo->eliminar(unaClave);
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
	}else {
		//TODO revisar
		setClave(registro,clave);
		if(archivo->buscar(registro)){
			archivo->modificar(registro);
		}else return false;
	}
	archivo->posicionarComponente(referencia);
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
	if(indice!=NULL){
		if(!indice->BuscarReferencia(clave,&referencia))
			return false;
		if(admin.acceder(referencia)){
			buffer->posicionarComponente(admin.getPosicionEnBuffer());
			return buffer->obtener(registro);
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
	admin.insertar(refArchivo);
	size_t posicionBuffer=admin.getPosicionEnBuffer();
	archivo->posicionarComponente(refArchivo);
	//TODO q pasa si strategia=NULL
	Componente *componente=archivo->getEstrategia()->getComponenteUsado();
	archivo->leer(componente);
	buffer->posicionarComponente(posicionBuffer);
	buffer->escribir(componente);
}
void EREscrituraDirecta::actualizarIndice(Cambio cambio){
	if(indice==NULL)return;
		switch(cambio.operacion){
		case Cambio::Alta : indice->insertar(cambio.referencia,&cambio.clave); break;
		case Cambio::Baja : indice->eliminar(&cambio.clave); break;
		case Cambio::Reubicacion : indice->modificar(&cambio.clave,cambio.referencia); break;
		default:break;
	}
}
void EREscrituraDirecta::actualizarBuffer(Cambio cambio){
	size_t posicionBuffer;
	Componente* componente;
	switch(cambio.operacion){
	case Cambio::Alta :
		insertarEnBuffer(cambio.referencia);
		break;
	case Cambio::Modificacion:
	case Cambio::Baja :
		if(admin.acceder(cambio.referencia)){
			posicionBuffer=admin.getPosicionEnBuffer();
			archivo->posicionarComponente(cambio.referencia);
			componente=archivo->getEstrategia()->getComponenteUsado()->clonar();
			archivo->leer(componente);
			buffer->posicionarComponente(posicionBuffer);
			buffer->escribir(componente);
			delete componente;
		}
		break;
	case Cambio::Reubicacion :
		Referencia posicionAnterior;
		indice->BuscarReferencia(&cambio.clave,&posicionAnterior);
		/* si el Componente de la posicoin previa esta en el buffer la actualizo*/
		if(admin.acceder(posicionAnterior)){
			posicionBuffer=admin.getPosicionEnBuffer();
			archivo->posicionarComponente(posicionAnterior);
			componente=archivo->getEstrategia()->getComponenteUsado()->clonar();
			archivo->leer(componente);
			buffer->posicionarComponente(posicionBuffer);
			buffer->escribir(componente);
			delete componente;
		}
		/*actualizo la nueva posicion si esta en el buffer*/
		if(!admin.acceder(cambio.referencia))
			admin.insertar(cambio.referencia);
		posicionBuffer=admin.getPosicionEnBuffer();
		archivo->posicionarComponente(cambio.referencia);
		componente=archivo->getEstrategia()->getComponenteUsado()->clonar();
		archivo->leer(componente);
		buffer->posicionarComponente(posicionBuffer);
		buffer->escribir(componente);
		delete componente;
		break;
	default:break;
	}

}
/*---------------------------------------------------------------------------------------------------------------*/
AdministradorDeBuffer::AdministradorDeBuffer(){
	tope=tablaArchivoBuffer.end();
	posicionEnTabla=tablaArchivoBuffer.end();
	capacidad=0;
}
AdministradorDeBuffer::~AdministradorDeBuffer(){
	for(std::map<size_t,NodoArchivoBuffer*>::iterator i=tablaArchivoBuffer.begin();i!=tablaArchivoBuffer.end();i++)
		delete i->second;
}
size_t AdministradorDeBuffer::getPosicionEnBuffer(){
	return posicionEnTabla->second->posicionBuffer;
}

bool AdministradorDeBuffer::buscar(size_t posicionArchivo){
	posicionEnTabla=tablaArchivoBuffer.find(posicionArchivo);
	return posicionEnTabla!=tablaArchivoBuffer.end();
}
NodoArchivoBuffer* AdministradorDeBuffer::at(size_t pos){
	if(pos<capacidad){
		return tablaArchivoBuffer[pos];
	}
	return NULL;
}
size_t AdministradorDeBuffer::size(){
	return tablaArchivoBuffer.size();
}
/*void AdministradorDeBuffer::promover(PNodoSiguiente& promovido){
	PNodoSiguiente mayor=tope;
	PNodoSiguiente menor=promovido->second->siguiente;;
	if(menor!=tablaArchivoBuffer.end()){
		if(promovido==tope){
			tope=menor;
		}else{
			while(mayor->second->siguiente!=promovido && mayor->second->siguiente!=tablaArchivoBuffer.end()){
				mayor=mayor->second->siguiente;
			};
			mayor->second->siguiente=menor;
		};
		while(mayor->second->siguiente!=tablaArchivoBuffer.end()){
			mayor=mayor->second->siguiente;
		}
		mayor->second->siguiente=promovido;
		promovido->second->siguiente=tablaArchivoBuffer.end();
	};
	posicionEnTabla=promovido;
}*/
void AdministradorDeBuffer::promoverAprimero(PNodoSiguiente &promovido){
	PNodoSiguiente mayor=tope;
	while(mayor->second->siguiente!=tablaArchivoBuffer.end()){
		mayor=mayor->second->siguiente;
	}
	mayor->second->siguiente=promovido;
	promovido->second->siguiente=tablaArchivoBuffer.end();
}
void AdministradorDeBuffer::insertar(size_t posicionArchivo){
	NodoArchivoBuffer*nuevo;
	PNodoSiguiente aux;
	if(tablaArchivoBuffer.size() < capacidad){
		nuevo=new NodoArchivoBuffer;
		nuevo->siguiente=tope;
		nuevo->posicionBuffer=tablaArchivoBuffer.size();
		tope=tablaArchivoBuffer.insert( std::pair<size_t, NodoArchivoBuffer* > (posicionArchivo,nuevo)).first;
		aux=tope;
	}else{
		aux=tablaArchivoBuffer.find(tope->first);
		nuevo=aux->second;
		tablaArchivoBuffer.erase(aux);
		aux=tablaArchivoBuffer.insert(std::pair<size_t, NodoArchivoBuffer* > (posicionArchivo,nuevo)).first;
	}
	promoverAprimero(aux);
	posicionEnTabla=aux;
}
bool AdministradorDeBuffer::acceder(size_t posicionArchivo){
	if(buscar(posicionArchivo)){
		promoverAprimero(posicionEnTabla);
		return true;
	}
	return false;
}

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






