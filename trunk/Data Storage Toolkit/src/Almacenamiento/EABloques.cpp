/*
 * EABloques.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */
#include "EABloques.h"
#include <iostream>

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
		almacen->escribir( (char*)&siguienteLibre , sizeof(siguienteLibre) );
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
	siguienteLibre=0;
	return anterior;
};
Almacenamiento* EABloques::abrir(Almacenamiento*almacen,const char*rutaArchivoEspacios){
	Almacenamiento*anterior=this->almacen;
	this->almacen=almacen;
	finalizarAlamcenamiento();
	archivoEspacioLibre.open(rutaArchivoEspacios,std::fstream::binary | std::fstream::in| std::fstream::out );
	this->almacen->posicionar(0);
	this->almacen->leer((char*)&siguienteLibre,sizeof(siguienteLibre));
	nroRegistro=0;
	return anterior;
};
bool EABloques::posicionarComponente(size_t nroCompuesto){
	if(nroCompuesto<siguienteLibre){
		nroBloque=nroCompuesto;
		almacen->posicionar(capacBloque*nroBloque+sizeof(siguienteLibre));
		return true;
	}return false;
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
	if(bloque!=NULL&&nroBloque<=siguienteLibre){
		if(bloque->tamanioSerializado()<=capacBloque){
			escribir(bloque);
			if(logActivo){
				for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
					clave->set((Registro*)bloque->get(i));
					pushCambio(Cambio(clave,nroBloque,Cambio::Alta));
				}
			}
			return true;
		};
	};
	return false;
};

bool EABloques::leer(Componente*compuesto){
	Bloque*bloque=dynamic_cast<Bloque*>(compuesto);
	if(bloque!=NULL &&nroBloque<siguienteLibre){
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
	while(!encontrado && nroBloque<siguienteLibre && !archivoEspacioLibre.eof()){
		archivoEspacioLibre.read((char*)&libres,sizeof(Ttamanio));
		Ttamanio disponibles=libres-(1-porcCarga)*capacBloque;
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
		size_t posInsercion=buscarEspacioLibre(registro->tamanioSerializado(),encontrado);
		if(encontrado){
			posicionarComponente(posInsercion);
			leer(bloque);
			bloque->agregar(componente);
		}else{
			bloque->inicializar(componente);
			siguienteLibre++;
		}
		posicionarComponente(posInsercion);
		escribir(bloque);

		if(logActivo){
			clave->set(registro);
			pushCambio(Cambio(clave,nroBloque,Cambio::Alta));
		}
	}
	return true;
};



bool EABloques::modificar(Componente*componente){
	size_t posModificado=nroBloque;
	if(nroBloque<siguienteLibre){
		leer(bloque);
		Ttamanio nroComponente=0;
		if(buscarComponente((Registro*)componente,nroComponente)){
			Ttamanio tamanio;
			tamanio=bloque->get(nroComponente)->tamanioSerializado()-componente->tamanioSerializado();
			if(tamanio>libres){
				bloque->eliminar(nroComponente);

				posicionarComponente(posModificado);
				escribir(bloque);
				tamanio=componente->tamanioSerializado();

				bool encontrado;
				posModificado=buscarEspacioLibre(tamanio,encontrado);
				if(encontrado){
					posicionarComponente(posModificado);
					leer(bloque);
					bloque->agregar(componente);
				}else{
					bloque->inicializar(componente);
					posModificado=siguienteLibre;
					siguienteLibre++;
				}
				if(logActivo){
					clave->set((Registro*)componente);
					pushCambio(Cambio(clave,posModificado,Cambio::Reubicacion));
				}
			}else{
				Componente* eliminado=bloque->reemplazar(componente,nroComponente);
				if(eliminado!=NULL)
					delete eliminado;
			}
			posicionarComponente(posModificado);
			escribir(bloque);
			return true;
		}
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
	size_t posBorrado=nroBloque;
	if(nroBloque<siguienteLibre){
		leer(bloque);
		Ttamanio nroComponente;
		if(buscarComponente((Registro*)componente,nroComponente)){
			if(bloque->cantidadComponentes()<=1){
				if(posBorrado<siguienteLibre-1){
					posicionarComponente(siguienteLibre-1);
					leer(bloque);
					if(logActivo){
						for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
							clave->set((Registro*)bloque->get(i));
							pushCambio(Cambio(clave,nroBloque,Cambio::Reubicacion));
						}
					}
				}
				siguienteLibre--;
			}else
				bloque->eliminar(nroComponente);
			if(posicionarComponente(posBorrado))
				escribir(bloque);
			if(logActivo){
				clave->set((Registro*)componente);
				pushCambio(Cambio(clave,nroBloque,Cambio::Baja));
			}
			return true;
		}
	}
	return false;
};
bool EABloques::siguiente(Componente*componente){
	if(nroBloque==0){
		leer(bloque);
		nroRegistro=0;
	}
	if(nroRegistro>= bloque->cantidadComponentes()){
		if(almacen->fin() || nroBloque>=siguienteLibre){
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
	if(nroBloque<siguienteLibre){
		leer(bloque);
		if(!buscarComponente((Registro*)componente,nroRegistro))
			return false;

		std::stringbuf buf(std::ios_base::binary | std::ios_base::in | std::ios_base::out );
		buf.pubsetbuf(bloqueSerializado,capacBloque);
		buf.pubseekpos(0);

		bloque->get(nroRegistro)->serializar(buf);
		buf.pubseekpos(0);

		componente->deserializar(buf);
		nroRegistro++;
		return true;
	}
	return false;
};
size_t EABloques::posicionComponente(){
	return nroBloque;
};
