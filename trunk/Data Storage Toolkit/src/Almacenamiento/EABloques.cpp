/*
 * EABloques.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */
#include "EABloques.h"

EABloques::EABloques(Bloque* tipoBloque,Ttamanio tamanioBloque) {
	almacen=NULL;
	this->bloque=(Bloque*)tipoBloque->clonar();
	capacBloque=tamanioBloque;
	bloqueSerializado=new char[tamanioBloque];
	porcCarga=0.8;
};

EABloques::~EABloques() {
	delete[] bloqueSerializado;
	delete bloque;
	archivoEspacioLibre.close();
};
void EABloques::finalizarAlamcenamiento(){
	if(archivoEspacioLibre.is_open()){
		almacen->escribir( (char*)&cantidadBloques , sizeof(cantidadBloques) );
		archivoEspacioLibre.close();
	}
};
Almacenamiento* EABloques::crear(Almacenamiento*almacen,const char*rutaArchivoEspacios){
	Almacenamiento*anterior=this->almacen;
	this->almacen=almacen;
	finalizarAlamcenamiento();
	archivoEspacioLibre.open(rutaArchivoEspacios,std::fstream::binary | std::fstream::out |std::fstream::trunc);
	archivoEspacioLibre.close();
	archivoEspacioLibre.open(rutaArchivoEspacios,std::fstream::binary | std::fstream::in| std::fstream::out|std::fstream::trunc );
	//todo archivoEspacioLibre.write((char*)&capacBloque,sizeof(capacBloque));
	archivoEspacioLibre.get();
	archivoEspacioLibre.seekg(0);
	nroRegistro=0;
	cantidadBloques=0;
	return anterior;
};
Almacenamiento* EABloques::abrir(Almacenamiento*almacen,const char*rutaArchivoEspacios){
	Almacenamiento*anterior=this->almacen;
	this->almacen=almacen;
	finalizarAlamcenamiento();
	archivoEspacioLibre.open(rutaArchivoEspacios,std::fstream::binary | std::fstream::in| std::fstream::out );
	this->almacen->posicionar(0);
	this->almacen->leer((char*)&cantidadBloques,sizeof(cantidadBloques));
	nroRegistro=0;
	return anterior;
};
bool EABloques::posicionarComponente(size_t nroCompuesto){
	if(nroCompuesto>=cantidadBloques)return false;
	nroBloque=nroCompuesto;
	almacen->posicionar(capacBloque*nroBloque+sizeof(cantidadBloques));
	return true;
};
void EABloques::leer(Bloque*bloque){
	/*leo el bloque*/
	almacen->leer(bloqueSerializado,capacBloque);
	/*leo espacio libres del bloque*/
	archivoEspacioLibre.seekg(nroBloque*sizeof(Ttamanio));
	archivoEspacioLibre.read((char*)&libres,sizeof(Ttamanio));
	/*deserializo*/
	std::stringbuf buf;
	buf.pubsetbuf(bloqueSerializado,capacBloque);
	buf.pubseekpos(0,std::ios_base::binary | std::ios_base::in);
	bloque->deserializar(buf);
	nroBloque++;
};
void EABloques::escribir(Bloque*bloque){
	/*Serializo*/
	std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
	buf.pubsetbuf(bloqueSerializado,capacBloque);
	bloque->serializar(buf);
	/*Actualizo archivo de espacios libres*/
	libres=capacBloque-bloque->tamanioSerializado();
	archivoEspacioLibre.seekp( nroBloque*sizeof(Ttamanio) , std::fstream::beg );
	archivoEspacioLibre.write((char*)&libres,sizeof(Ttamanio));
	/*Escribo el bloque*/
	almacen->escribir(bloqueSerializado,capacBloque);
	nroBloque++;
};
bool EABloques::escribir(Componente*compuesto){
	Bloque*bloque=dynamic_cast<Bloque*>(compuesto);
	if(bloque!=NULL){
		if(bloque->tamanioSerializado()<=capacBloque){
			escribir(bloque);
			if(logActivo){
				for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
					clave->set((Registro*)bloque->get(i));
					cambiosLog.push(new Cambio(*clave,nroBloque,Cambio::Alta));
				}
			}
			return true;
		};
	};
	return false;
};

bool EABloques::leer(Componente*compuesto){
	Bloque*bloque=dynamic_cast<Bloque*>(compuesto);
	if(bloque!=NULL && !almacen->fin()){
		leer(bloque);
		return true;
	}
	return false;
};
size_t EABloques::buscarEspacioLibre(Ttamanio espacio,bool& encontrado){
	size_t nroBloque=0;
	libres=capacBloque;
	archivoEspacioLibre.seekg(0);
	encontrado=false;
	while(!encontrado && !archivoEspacioLibre.eof()){
		archivoEspacioLibre.read((char*)&libres,sizeof(Ttamanio));
		Ttamanio disponibles=0;
		disponibles=libres-(1-porcCarga)*capacBloque;
		encontrado=disponibles>espacio;
		if(!encontrado)nroBloque++;
	};
	archivoEspacioLibre.clear();
	return nroBloque;
};

bool EABloques::insertar(Componente*componente){
	bool encontrado;
	Registro*registro=dynamic_cast<Registro*>(componente);
	if(registro!=NULL){
		Ttamanio espacioNecesario=registro->tamanioSerializado();
		nroBloque=buscarEspacioLibre(espacioNecesario,encontrado);
		if(encontrado){
			posicionarComponente(nroBloque);
			leer(bloque);
			nroBloque--;
			bloque->agregar(componente);
		}else{
			bloque->inicializar(componente);
			cantidadBloques++;
		}
		if(logActivo){
			clave->set(registro);
			cambiosLog.push(new Cambio(*clave,nroBloque,Cambio::Alta));
		}
		posicionarComponente(nroBloque);
		escribir(bloque);
	}
	return true;
};



bool EABloques::modificar(Componente*componente){
	leer(bloque);
	nroBloque--;
	Ttamanio nroComponente=0;
	if(buscarComponente((Registro*)componente,nroComponente)){
		Ttamanio tamanio;
		tamanio=bloque->get(nroComponente)->tamanioSerializado()-componente->tamanioSerializado();
		if(tamanio>libres){
			bloque->eliminar(nroComponente);
			posicionarComponente(nroBloque);
			escribir(bloque);
			tamanio=componente->tamanioSerializado();
			bool encontrado;
			nroBloque=buscarEspacioLibre(tamanio,encontrado);
			if(encontrado){
				posicionarComponente(nroBloque);
				leer(bloque);
				nroBloque--;
				bloque->agregar(componente);
			}else{
				bloque->inicializar(componente);
				cantidadBloques++;
			}
			if(logActivo){
				clave->set((Registro*)componente);
				cambiosLog.push(new Cambio(*clave,nroBloque,Cambio::Reubicacion));
			}
		}else{
			Componente* eliminado=bloque->reemplazar(componente,nroComponente);
			if(eliminado!=NULL)	delete eliminado;
		}
		posicionarComponente(nroBloque);
		escribir(bloque);
		return true;
	}
	return false;
};
bool EABloques::buscarComponente(Registro*registro,Ttamanio & posicion){
	clave->set(registro);
	Clave*aux=clave->clonarce();
	for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
		clave->set((Registro*)bloque->get(i));
		if(comparador->Comparar(clave,aux)==0){
		   posicion=i;
		   delete aux;
		   return true;
		}
	}
	delete aux;
	return false;
};
bool EABloques::eliminar(Componente*componente){
	leer(bloque);
	nroBloque--;
	Ttamanio nroComponente;
	if(!buscarComponente((Registro*)componente,nroComponente)) return false;
	if(logActivo){
		clave->set((Registro*)componente);
		cambiosLog.push(new Cambio(*clave,nroBloque,Cambio::Baja));
	}
	if(bloque->cantidadComponentes()==1){
		size_t posBorrado=nroBloque;
		cantidadBloques--;
		posicionarComponente(cantidadBloques);
		leer(bloque);
		if(logActivo){
			for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
				clave->set((Registro*)bloque->get(i));
				cambiosLog.push(new Cambio(*clave,nroBloque,Cambio::Reubicacion));
			}
		}
		nroBloque=posBorrado;
	}else bloque->eliminar(nroComponente);;
	posicionarComponente(nroBloque);
	escribir(bloque);
	return true;
};
bool EABloques::siguiente(Componente*componente){
	if(nroBloque==0)
		leer(bloque);
	if(nroRegistro>= bloque->cantidadComponentes()){
		if(almacen->fin() || nroBloque>=cantidadBloques){
			return false;
		}
		leer(bloque);
		nroRegistro=0;
	}
	std::stringbuf buf(std::ios_base::binary | std::ios_base::in | std::ios_base::out );
	buf.pubsetbuf(bloqueSerializado,capacBloque);
	buf.pubseekpos(0);

	bloque->get(nroRegistro)->serializar(buf);
	buf.pubseekpos(0);

	componente->deserializar(buf);
	nroRegistro++;
	return true;
};
Componente *EABloques::getComponente(){
	return bloque;
}

bool EABloques::obtener(Componente*componente){
	leer(bloque);
	Ttamanio nroComp;
	if(!buscarComponente((Registro*)componente,nroComp)) return false;
	return true;
};
