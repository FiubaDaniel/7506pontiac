#include "EREscrituraDirecta.h"

EREscrituraDirecta::EREscrituraDirecta(Almacenamiento *buffer): admin(10){
	this->buffer=buffer;
}

EREscrituraDirecta::~EREscrituraDirecta(){}

bool EREscrituraDirecta::insertar(Registro* registro){
	//TODO clave->set(registro);
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
	//TODO registro.set(clave);
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
	//TODO clave->set(registro);
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
	size_t posicioBuffer=posicionEnBuffer(referencia);
	if(referencia!=0){
		referencia--;
		estrategiaBuffer->posicionarComponente(posicioBuffer);
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
	admin.insertar(refArchivo);
	size_t posicionBuffer=admin.getPosicionEnBuffer();
	//escribo en el buffer
	estrategiaArchivo->posicionarComponente(refArchivo);
	Componente *componente=estrategiaArchivo->getComponente();
	estrategiaArchivo->leer(componente);
	estrategiaBuffer->posicionarComponente(posicionBuffer);
	estrategiaBuffer->escribir(componente);
};

size_t EREscrituraDirecta::posicionEnBuffer(size_t posicionArchivo){

	return 0;
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
		case Cambio::Baja :
			posicionBuffer=posicionEnBuffer(cambio.referencia);
			if(posicionBuffer!=0){
				posicionBuffer--;
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
			posicionBuffer=posicionEnBuffer(posicionAnterior);
			if(posicionBuffer!=0){
				posicionBuffer--;
				estrategiaArchivo->posicionarComponente(posicionAnterior);
				componente=estrategiaArchivo->getComponente();
				estrategiaArchivo->leer(componente);
				estrategiaBuffer->posicionarComponente(posicionBuffer);
				estrategiaBuffer->escribir(componente);
			}
			posicionBuffer=posicionEnBuffer(cambio.referencia);
			if(posicionBuffer!=0){
				posicionBuffer--;
				estrategiaArchivo->posicionarComponente(posicionAnterior);
				componente=estrategiaArchivo->getComponente();
				estrategiaArchivo->leer(componente);
				estrategiaBuffer->posicionarComponente(posicionBuffer);
				estrategiaBuffer->escribir(componente);
			};
			break;
	}
};
/*---------------------------------------------------------------------------------------------------------------*/
size_t AdministradorDeBuffer::getPosicionEnBuffer(){
	return tablaArchivoBuffer[posicionEnTabla].posicionBuffer;
};

bool AdministradorDeBuffer::buscar(size_t posicionArchivo){
	size_t menor,mayor;
	menor=0;
	mayor=ultimo;
	do{
		posicionEnTabla=(mayor+menor)/2;
		if(tablaArchivoBuffer[posicionEnTabla].posicionArchivo > posicionArchivo) mayor=posicionEnTabla-1;
		else if(tablaArchivoBuffer[posicionEnTabla].posicionArchivo < posicionArchivo) menor=posicionEnTabla+1;
		else return true;
	}while(menor<=mayor);
	return false;
}

void AdministradorDeBuffer::promover(NodoArchivoBuffer*promovido){
	NodoArchivoBuffer*mayor=tope;
	NodoArchivoBuffer*menor=promovido->siguiente;;
	if(menor!=NULL){
		if(promovido==tope){
			tope=menor;
		}else{
			while(mayor->siguiente!=promovido && mayor->siguiente!=NULL){
				mayor=mayor->siguiente;
			};
			mayor->siguiente=menor;
		};
		promovido->siguiente=menor->siguiente;
		menor->siguiente=promovido;
	};
};

AdministradorDeBuffer::AdministradorDeBuffer(size_t maximo){
	tope=NULL;
	capacidad=maximo;
	ultimo=0;
	tablaArchivoBuffer=new NodoArchivoBuffer[maximo];
}
AdministradorDeBuffer::~AdministradorDeBuffer(){
	delete[] tablaArchivoBuffer;
};

NodoArchivoBuffer* AdministradorDeBuffer::at(size_t pos){
	if(pos<capacidad)
		return &tablaArchivoBuffer[pos];
	return NULL;
}

size_t AdministradorDeBuffer::size(){return ultimo;}

void AdministradorDeBuffer::insertar(size_t posicionArchivo){
	if(ultimo< capacidad){
		if(tope==NULL)
			tablaArchivoBuffer[ultimo].siguiente=NULL;
		else
			tablaArchivoBuffer[ultimo].siguiente=tablaArchivoBuffer+ultimo-1;
		tablaArchivoBuffer[ultimo].posicionArchivo=posicionArchivo;
		tablaArchivoBuffer[ultimo].posicionBuffer=ultimo;
		tope=tablaArchivoBuffer+ultimo;
		promover(tope);
		ultimo++;
	}else {
		if(buscar(tope->posicionArchivo)){
			tope->posicionArchivo=posicionArchivo;
			promover(tope);
		}
	}
}
bool AdministradorDeBuffer::acceder(size_t posicionArchivo){
	if(buscar(posicionArchivo)){
		promover(&tablaArchivoBuffer[posicionEnTabla]);
		return true;
	}
	return false;
}




