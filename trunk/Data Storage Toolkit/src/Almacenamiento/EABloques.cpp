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
		archivoEspacioLibre.close();
	}
};
Almacenamiento* EABloques::nuevo(Almacenamiento*almacen,const char*rutaArchivoEspacios){
	Almacenamiento*anterior=this->almacen;
	this->almacen=almacen;
	finalizarAlamcenamiento();
	archivoEspacioLibre.open(rutaArchivoEspacios,std::fstream::binary | std::fstream::out );
	archivoEspacioLibre.close();
	archivoEspacioLibre.open(rutaArchivoEspacios,std::fstream::binary | std::fstream::in| std::fstream::out );
	return anterior;
};
Almacenamiento* EABloques::abrir(Almacenamiento*almacen,const char*rutaArchivoEspacios){
	Almacenamiento*anterior=this->almacen;
	this->almacen=almacen;
	finalizarAlamcenamiento();
	archivoEspacioLibre.open(rutaArchivoEspacios,std::fstream::binary | std::fstream::in| std::fstream::out );
	return anterior;
};
bool EABloques::posicionarComponente(size_t nroCompuesto){
	nroBloque=nroCompuesto;
	almacen->posicionar(capacBloque*nroBloque);
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
	archivoEspacioLibre.seekp(nroBloque*sizeof(Ttamanio));
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
					Cambio cambio(clave,nroBloque,Cambio::Alta);
					cambiosLog.push(cambio);
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
	archivoEspacioLibre.seekg(std::fstream::beg);
	do{
		archivoEspacioLibre.read((char*)&libres,sizeof(Ttamanio));
		Ttamanio disponibles=libres-(1-porcCarga)*capacBloque;
		if(disponibles>espacio) encontrado=true;
		else nroBloque++;
	}while(!encontrado && archivoEspacioLibre.eof());
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
		}
		if(logActivo){
			clave->set(registro);
			Cambio cambio(clave,nroBloque,Cambio::Alta);
			cambiosLog.push(cambio);
		}
		posicionarComponente(nroBloque);
		escribir(bloque);
	}
	//todo
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
			}else bloque->inicializar(componente);

			if(logActivo){
				clave->set((Registro*)componente);
				Cambio cambio(clave,nroBloque,Cambio::Reubicacion);
				cambiosLog.push(cambio);
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
	bloque->eliminar(nroComponente);
	if(logActivo){
		clave->set((Registro*)componente);
		Cambio cambio(clave,nroBloque,Cambio::Baja);
		cambiosLog.push(cambio);
	}
	posicionarComponente(nroBloque);
	escribir(bloque);
	return true;
};
bool EABloques::siguiente(Componente*componente){
	if(nroRegistro>=bloque->cantidadComponentes()){
		if(almacen->fin())return false;
		leer(bloque);
	}
	std::stringbuf buf(std::ios_base::binary | std::ios_base::in );
	buf.pubsetbuf(bloqueSerializado,capacBloque);
	buf.pubseekpos(std::ios_base::beg);
	bloque->get(nroRegistro)->serializar(buf);
	buf.pubseekpos(std::ios_base::beg);
	componente->deserializar(buf);
	return true;
};
bool EABloques::obtener(Componente*componente){
	leer(bloque);
	Ttamanio nroComp;
	if(!buscarComponente((Registro*)componente,nroComp)) return false;
	return true;
};
