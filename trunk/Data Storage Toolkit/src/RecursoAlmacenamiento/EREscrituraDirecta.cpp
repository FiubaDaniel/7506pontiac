#include "EREscrituraDirecta.h"

EREscrituraDirecta::EREscrituraDirecta(Almacenamiento *buffer): admin(10){
	this->buffer=buffer;
}

EREscrituraDirecta::~EREscrituraDirecta(){}
void EREscrituraDirecta::setClave(Registro*reg,Clave*clave){
	for(Ttamanio i=0;i<clave->getCantidadAtributos();i++){
		Atributo*att=clave->getAtributo(i);
		reg->get(att->getNombre())->set(att);
	}
};
bool EREscrituraDirecta::insertar(Registro* registro){
	clave->set(registro);
	Referencia referencia;
	if(indice->BuscarReferencia(clave,&referencia))
		return false;
	estrategiaArchivo->insertar(registro);
	while(!estrategiaArchivo->cambiosLog.empty()){
		Cambio cambio=estrategiaArchivo->cambiosLog.front();
		actualizarBuffer(cambio);
		actualizarIndice(cambio);
		estrategiaArchivo->cambiosLog.pop();
	};
	return true;
};
bool EREscrituraDirecta::eliminar(Clave* unaClave){
	Referencia referencia;
	if(!indice->BuscarReferencia(unaClave,&referencia))
		return false;
	estrategiaArchivo->posicionarComponente(referencia);
	setClave(registro,clave);
	estrategiaArchivo->eliminar(registro);
	indice->eliminar(unaClave);
	while(!estrategiaArchivo->cambiosLog.empty()){
		Cambio cambioEnArchivo=estrategiaArchivo->cambiosLog.front();
		actualizarBuffer(cambioEnArchivo);
		actualizarIndice(cambioEnArchivo);
		estrategiaArchivo->cambiosLog.pop();
	};
	return true;
};
bool EREscrituraDirecta::modificar(Clave* unaClave,Registro* registro){
	clave->set(registro);
	Referencia referencia;
	if(!indice->BuscarReferencia(clave,&referencia))
		return false;
	estrategiaArchivo->posicionarComponente(referencia);
	if(!estrategiaArchivo->modificar(registro))
		return false;
	while(!estrategiaArchivo->cambiosLog.empty()){
		Cambio cambio=estrategiaArchivo->cambiosLog.front();
		actualizarBuffer(cambio);
		actualizarIndice(cambio);
		estrategiaArchivo->cambiosLog.pop();
	};
	return true;
};


bool EREscrituraDirecta::obtener(Clave* unaClave,Registro*registro){
	Referencia referencia;
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
	};
	return false;
};
void EREscrituraDirecta::insertarEnBuffer(Referencia refArchivo){
	//TODO admin.insertar(refArchivo);
	size_t posicionBuffer=admin.getPosicionEnBuffer();
	estrategiaArchivo->posicionarComponente(refArchivo);
	Componente *componente=estrategiaArchivo->getComponente();
	estrategiaArchivo->leer(componente);
	estrategiaBuffer->posicionarComponente(posicionBuffer);
	estrategiaBuffer->escribir(componente);
};
void EREscrituraDirecta::actualizarIndice(Cambio cambio){
	switch(cambio.operacion){
		case Cambio::Alta : indice->insertar(cambio.referencia,cambio.clave); break;
		case Cambio::Baja : indice->eliminar(cambio.clave); break;
		case Cambio::Reubicacion : indice->modificar(cambio.clave,cambio.referencia); break;
	}
};
void EREscrituraDirecta::actualizarBuffer(Cambio cambio){
	size_t posicionBuffer;
	Componente* componente;
	switch(cambio.operacion){
		case Cambio::Alta :
			registro=NULL;
			insertarEnBuffer(cambio.referencia);
			break;
		case Cambio::Modificacion:
		case Cambio::Baja :
			if(admin.acceder(cambio.referencia)){
				posicionBuffer=admin.getPosicionEnBuffer();
				estrategiaArchivo->posicionarComponente(cambio.referencia);
				componente=estrategiaArchivo->getComponente();
				estrategiaArchivo->leer(componente);
				estrategiaBuffer->posicionarComponente(posicionBuffer);
				estrategiaBuffer->escribir(componente);
			}
			break;
		case Cambio::Reubicacion :
			Referencia posicionAnterior;
			indice->BuscarReferencia(cambio.clave,&posicionAnterior);
			/* si el Componente de la posicoin previa esta en el buffer la actualizo*/
			if(admin.acceder(posicionAnterior)){
				posicionBuffer=admin.getPosicionEnBuffer();
				estrategiaArchivo->posicionarComponente(posicionAnterior);
				componente=estrategiaArchivo->getComponente();
				estrategiaArchivo->leer(componente);
				estrategiaBuffer->posicionarComponente(posicionBuffer);
				estrategiaBuffer->escribir(componente);
			}
			/*actualizo la nueva posicion si esta en el buffer*/
			if(!admin.acceder(cambio.referencia))
				admin.insertar(cambio.referencia);
			posicionBuffer=admin.getPosicionEnBuffer();
			estrategiaArchivo->posicionarComponente(cambio.referencia);
			componente=estrategiaArchivo->getComponente();
			estrategiaArchivo->leer(componente);
			estrategiaBuffer->posicionarComponente(posicionBuffer);
			estrategiaBuffer->escribir(componente);
			break;
	}
};
/*---------------------------------------------------------------------------------------------------------------*/
AdministradorDeBuffer::AdministradorDeBuffer(size_t maximo){
	tope=tablaArchivoBuffer.end();
	posicionEnTabla=tablaArchivoBuffer.end();
	capacidad=maximo;
}
AdministradorDeBuffer::~AdministradorDeBuffer(){
	for(std::map<size_t,NodoArchivoBuffer*>::iterator i=tablaArchivoBuffer.begin();i!=tablaArchivoBuffer.end();i++)
		delete i->second;
};
size_t AdministradorDeBuffer::getPosicionEnBuffer(){
	return posicionEnTabla->second->posicionBuffer;
};

bool AdministradorDeBuffer::buscar(size_t posicionArchivo){
	posicionEnTabla=tablaArchivoBuffer.find(posicionArchivo);
	return posicionEnTabla!=tablaArchivoBuffer.end();

}


NodoArchivoBuffer* AdministradorDeBuffer::at(size_t pos){
	if(pos<capacidad){
		return tablaArchivoBuffer.at(pos);
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
};
void AdministradorDeBuffer::promoverAprimero(PNodoSiguiente &promovido){
	PNodoSiguiente mayor=tope;
	while(mayor->second->siguiente!=tablaArchivoBuffer.end()){
		mayor=mayor->second->siguiente;
	}
	mayor->second->siguiente=promovido;
	promovido->second->siguiente=tablaArchivoBuffer.end();
};
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
};
bool AdministradorDeBuffer::acceder(size_t posicionArchivo){
	if(buscar(posicionArchivo)){
		promover(posicionEnTabla);
		return true;
	}
	return false;
}




