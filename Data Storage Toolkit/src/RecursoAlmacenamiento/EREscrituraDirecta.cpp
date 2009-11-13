#include "EREscrituraDirecta.h"

EREscrituraDirecta::EREscrituraDirecta(EstrategiaIndice* indice,EstrategiaAlmacenamiento*estrategiaArchivo,EstrategiaAlmacenamiento*estrategiaBuffer,size_t cantidadElementoBuffer){
	this->estrategiaArchivo=estrategiaArchivo;
	this->estrategiaBuffer=estrategiaBuffer;
	this->indice=indice;
	this->estrategiaBuffer->logActivo=false;
	this->estrategiaArchivo->logActivo=true;
	admin.setCapacidad(cantidadElementoBuffer);
	registro=(Registro*)this->estrategiaArchivo->getRegistro()->clonar();
	clave=this->estrategiaArchivo->getClave();
}

EREscrituraDirecta::~EREscrituraDirecta(){
	delete registro;
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
		if(!estrategiaBuffer->buscar(registro)){
			estrategiaArchivo->logActivo=false;
			if(estrategiaArchivo->buscar(registro))
				return false;
			estrategiaArchivo->logActivo=true;
		}else return false;
	}
	if(!estrategiaArchivo->insertar(registro))return false;
	while(!estrategiaArchivo->NoHayMasCambios()){
		const Cambio* cambio=estrategiaArchivo->siguienteCambio();
		actualizarBuffer(*cambio);
		actualizarIndice(*cambio);
		estrategiaArchivo->pop();
	};
	return true;
}
bool EREscrituraDirecta::eliminar(Clave* unaClave){
	Referencia referencia;
	if(indice!=NULL){
		if(!indice->BuscarReferencia(unaClave,&referencia))
			return false;
	}else {
		estrategiaArchivo->logActivo=true;
		setClave(registro,clave);
		if(!estrategiaArchivo->buscar(registro))
			return false;
		referencia=estrategiaArchivo->siguienteCambio()->referencia;
		estrategiaArchivo->pop();
		estrategiaArchivo->logActivo=false;
	}
	estrategiaArchivo->posicionarComponente(referencia);
	setClave(registro,clave);
	if(!estrategiaArchivo->eliminar(registro))
		return false;
	while(!estrategiaArchivo->NoHayMasCambios()){
		const Cambio* cambio=estrategiaArchivo->siguienteCambio();
		actualizarBuffer(*cambio);
		actualizarIndice(*cambio);
		estrategiaArchivo->pop();
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
		estrategiaArchivo->logActivo=true;
		setClave(registro,clave);
		if(!estrategiaArchivo->buscar(registro))
			return false;
		referencia=estrategiaArchivo->siguienteCambio()->referencia;
		estrategiaArchivo->pop();
		estrategiaArchivo->logActivo=false;
	}
	estrategiaArchivo->posicionarComponente(referencia);
	if(!estrategiaArchivo->modificar(registro))
		return false;
	while(!estrategiaArchivo->NoHayMasCambios()){
		const Cambio* cambio=estrategiaArchivo->siguienteCambio();
		actualizarBuffer(*cambio);
		actualizarIndice(*cambio);
		estrategiaArchivo->pop();
	};
	return true;
}


bool EREscrituraDirecta::obtener(Clave* unaClave,Registro*registro){
	Referencia referencia;
	if(indice!=NULL){
		if(!indice->BuscarReferencia(clave,&referencia))
			return false;
		if(admin.acceder(referencia)){
			estrategiaBuffer->posicionarComponente(admin.getPosicionEnBuffer());
			return estrategiaBuffer->obtener(registro);
		}
		estrategiaArchivo->posicionarComponente(referencia);
		if(estrategiaArchivo->obtener(registro)){
			insertarEnBuffer(referencia);
			return true;
		}
	}else{
		if(estrategiaBuffer->buscar(registro))
			return true;
		estrategiaArchivo->logActivo=false;
		if(estrategiaArchivo->buscar(registro))
			return true;
		estrategiaArchivo->logActivo=true;
	}
	return false;
}
void EREscrituraDirecta::insertarEnBuffer(Referencia refArchivo){
	admin.insertar(refArchivo);
	size_t posicionBuffer=admin.getPosicionEnBuffer();
	estrategiaArchivo->posicionarComponente(refArchivo);
	Componente *componente=estrategiaArchivo->getComponente();
	estrategiaArchivo->leer(componente);
	estrategiaBuffer->posicionarComponente(posicionBuffer);
	estrategiaBuffer->escribir(componente);
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
				estrategiaArchivo->posicionarComponente(cambio.referencia);
				componente=estrategiaArchivo->getRegistro();
				estrategiaArchivo->leer(componente);
				estrategiaBuffer->posicionarComponente(posicionBuffer);
				estrategiaBuffer->escribir(componente);
			}
			break;
		case Cambio::Reubicacion :
			Referencia posicionAnterior;
			indice->BuscarReferencia(&cambio.clave,&posicionAnterior);
			/* si el Componente de la posicoin previa esta en el buffer la actualizo*/
			if(admin.acceder(posicionAnterior)){
				posicionBuffer=admin.getPosicionEnBuffer();
				estrategiaArchivo->posicionarComponente(posicionAnterior);
				componente=estrategiaArchivo->getRegistro();
				estrategiaArchivo->leer(componente);
				estrategiaBuffer->posicionarComponente(posicionBuffer);
				estrategiaBuffer->escribir(componente);
			}
			/*actualizo la nueva posicion si esta en el buffer*/
			if(!admin.acceder(cambio.referencia))
				admin.insertar(cambio.referencia);
			posicionBuffer=admin.getPosicionEnBuffer();
			estrategiaArchivo->posicionarComponente(cambio.referencia);
			componente=estrategiaArchivo->getRegistro();
			estrategiaArchivo->leer(componente);
			estrategiaBuffer->posicionarComponente(posicionBuffer);
			estrategiaBuffer->escribir(componente);
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
void AdministradorDeBuffer::promover(PNodoSiguiente& promovido){
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
}
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
	promover(aux);
	posicionEnTabla=aux;
}
bool AdministradorDeBuffer::acceder(size_t posicionArchivo){
	if(buscar(posicionArchivo)){
		promover(posicionEnTabla);
		return true;
	}
	return false;
}

EstrategiaIndice *EREscrituraDirecta::getIndice(){
	return indice;
}



EstrategiaAlmacenamiento *EREscrituraDirecta::getEstrategiaAlmacenamiento(){
	return estrategiaArchivo;
}
EstrategiaAlmacenamiento *EREscrituraDirecta::getEstrategiaBuffer(){
	return estrategiaBuffer;
}


void EREscrituraDirecta::setIndice(EstrategiaIndice *indice){
	this->indice=indice;
}

void EREscrituraDirecta::setEstrategiaBuffer(EstrategiaAlmacenamiento* estrategia){
	estrategiaBuffer=estrategia;
	registro=(Registro*)estrategia->getRegistro();
	clave=estrategia->getClave();
}

void EREscrituraDirecta::setEstrategiAlmacenamiento(EstrategiaAlmacenamiento *estrategia){
	estrategiaArchivo=estrategia;
	estrategiaArchivo->logActivo=true;
	registro=(Registro*)estrategia->getRegistro();
	clave=estrategia->getClave();
}






