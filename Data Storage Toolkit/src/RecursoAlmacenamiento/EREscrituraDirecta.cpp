#include "EREscrituraDirecta.h"

EREscrituraDirecta::EREscrituraDirecta(Almacenamiento *buffer){
	this->buffer=buffer;
}

EREscrituraDirecta::~EREscrituraDirecta(){}

bool EREscrituraDirecta::insertar(Registro* registro){
	clave->set(registro);
	Referencia referencia;
	if(indice->BuscarReferencia(*clave,&referencia))
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
	if(!indice->BuscarReferencia(*unaClave,&referencia))
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
	clave->set(registro);
	Referencia referencia;
	if(!indice->BuscarReferencia(*clave,&referencia))
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
	if(!indice->BuscarReferencia(*clave,&referencia))
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
/*	ParDireccionArchivoBuffer par;
	if(posicionesUsadasPorBuffer.size() < posicionesTotalesDelBuffer){
		si el buffer tiene espacion, el componente se copia al final
		par.posicionArchivo=refArchivo;
		par.posicionBuffer=posicionesUsadasPorBuffer.size();
		posicionesUsadasPorBuffer.push_back(par.posicionBuffer);
	}else{
		si el buffer esta lleno,se toma la ultima posicionUsadasPorBuffer
		 * la cual corresponde a la menos recientemente usada

		par.posicionBuffer=posicionesUsadasPorBuffer.back();
	}
	como la ultima posicion esta siendo usada , se promueve
	(*posicionesUsadasPorBuffer.end())=*(posicionesUsadasPorBuffer.end()-1);
	*posicionesUsadasPorBuffer.end()=par.posicionBuffer;
	escribo en el buffer
	estrategiaArchivo->posicionarComponente(par.posicionBuffer);
	Componente *componente=estrategiaArchivo->getComponente();
	estrategiaArchivo->leer(componente);
	estrategiaBuffer->posicionarComponente(par.posicionBuffer);
	estrategiaBuffer->escribir(componente);*/
};

size_t EREscrituraDirecta::posicionEnBuffer(size_t posicionArchivo){
	std::list<ParDireccionArchivoBuffer>::iterator i;
	size_t menor, medio,mayor;
	menor=0;
	//TODO mayor=tablaArchivoBuffer.size();
	do{
		medio=(mayor+menor)/2;
		if((*i).posicionArchivo > posicionArchivo) mayor=medio-1;
		else if((*i).posicionArchivo< posicionArchivo) menor=medio+1;
		else return (*i).posicionBuffer+1;
	}while(menor<mayor);
	return 0;
};
void EREscrituraDirecta::actualizarIndice(Cambio cambio){
	switch(cambio.operacion){
		case Cambio::Alta : indice->insertar(cambio.referencia,cambio.clave); break;
		case Cambio::Baja : indice->eliminar(cambio.clave); break;
		case Cambio::Reubicacion : indice->modificar(*cambio.clave,cambio.referencia); break;
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
			indice->BuscarReferencia(*cambio.clave,&posicionAnterior);
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

size_t AdministradorBuffer::getPosicionEnBuffer(){
	return tablaArchivoBuffer.at(posicionEnTabla).posicionBuffer;
};

bool AdministradorBuffer::buscar(size_t posicionArchivo){
	size_t menor,mayor;
	menor=0;
	mayor=tablaArchivoBuffer.size();
	do{
		posicionEnTabla=(mayor+menor)/2;
		if(tablaArchivoBuffer.at(posicionEnTabla).posicionArchivo > posicionArchivo) mayor=posicionEnTabla-1;
		else if(tablaArchivoBuffer.at(posicionEnTabla).posicionArchivo < posicionArchivo) menor=posicionEnTabla+1;
		else return true;
	}while(menor<mayor);
	return false;
}

void AdministradorBuffer::insertar(size_t posicionArchivo){

}
bool AdministradorBuffer::acceder(size_t posicionArchivo){
	if(buscar(posicionArchivo)){

		return true;
	}
	return false;
}




