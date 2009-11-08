/*
 * EABloques.cpp
 *
 *  Created on: 07/10/2009
 *      Author: paulo
 */
#include "EABloques.h"
#include <iostream>

EABloques::EABloques(){
	bloque=NULL;
	almacen=NULL;
	nroRegistro=0;
	siguienteLibre=0;
	nroBloque=0;
	libres=0;
	capacBloque=0;
	bloqueSerializado=0;
	porcCarga=0;
}

EABloques::EABloques(Registro* tipoRegistro,Ttamanio tamanioBloque,double porcCarga){
	almacen=NULL;
	this->bloque=new Bloque(tipoRegistro);
	capacBloque=tamanioBloque;
	bloqueSerializado=new char[tamanioBloque];
	this->porcCarga=porcCarga;
}

EABloques::~EABloques() {
	delete[] bloqueSerializado;
	delete bloque;
	archivoEspacioLibre.close();
}
void EABloques::setRegistro(Registro*registro){
	if(bloque!=NULL){
		bloque->inicializar(registro);
	}else bloque=new Bloque(registro);
};
void EABloques::finalizarAlamcenamiento(){
	archivoEspacioLibre.close();
	almacen->escribir( (char*)&capacBloque , sizeof(capacBloque) );
	almacen->escribir( (char*)&siguienteLibre , sizeof(siguienteLibre) );
}
bool EABloques::crear(Almacenamiento*almacen){
	this->almacen=almacen;
	finalizarAlamcenamiento();
	std::string ruta=almacen->getNombre()+".bloques";
	nroRegistro=0;
	nroBloque=0;
	siguienteLibre=0;
	archivoEspacioLibre.open(ruta.c_str(),std::fstream::binary | std::fstream::out |std::fstream::trunc);
	archivoEspacioLibre.close();
	archivoEspacioLibre.open(ruta.c_str(),std::fstream::binary | std::fstream::in| std::fstream::out|std::fstream::trunc );
	almacen->escribir( (char*)&capacBloque , sizeof(capacBloque) );
	almacen->escribir( (char*)&siguienteLibre , sizeof(siguienteLibre) );
	return archivoEspacioLibre.is_open();
}
void EABloques::cerrar(){
	finalizarAlamcenamiento();
	almacen->cerrar();
}
bool EABloques::abrir(Almacenamiento*almacen){
	this->almacen=almacen;
	finalizarAlamcenamiento();
	std::string ruta=almacen->getNombre()+".bloques";
	archivoEspacioLibre.open(ruta.c_str(),std::fstream::binary | std::fstream::in| std::fstream::out );
	this->almacen->posicionar(0);
	this->almacen->leer((char*)&capacBloque,sizeof(siguienteLibre));
	this->almacen->leer((char*)&siguienteLibre,sizeof(siguienteLibre));
	nroRegistro=0;
	return archivoEspacioLibre.is_open();
}
bool EABloques::posicionarComponente(size_t nroCompuesto){
	if(nroCompuesto<siguienteLibre){
		nroBloque=nroCompuesto;
		almacen->posicionar(capacBloque*nroBloque+sizeof(siguienteLibre)+sizeof(capacBloque));
		almacen->reiniciar();
		nroRegistro=0;
		return true;
	}return false;
}
bool EABloques::leerBloque(Bloque*bloque){
	/*leo el bloque*/
	almacen->leer(bloqueSerializado,capacBloque);
	if(almacen->fin()){
		almacen->reiniciar();
		return false;
	}
	/*leo espacio libres del bloque*/
	archivoEspacioLibre.seekg(nroBloque*sizeof(Ttamanio));
	archivoEspacioLibre.read((char*)&libres,sizeof(Ttamanio));
	/*deserializo*/
	std::stringbuf buf;
	buf.pubsetbuf(bloqueSerializado,capacBloque);
	buf.pubseekpos(0,std::ios_base::binary | std::ios_base::in);
	bloque->deserializar(buf);
	nroBloque++;
	return true;
}
bool EABloques::escribirBloque(Bloque*bloque){
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
	return true;
}
bool EABloques::escribir(Componente*compuesto){
	Bloque*bloque=dynamic_cast<Bloque*>(compuesto);
	if(bloque!=NULL&&nroBloque<=siguienteLibre){
		if(bloque->tamanioSerializado()<=capacBloque){
			escribirBloque(bloque);
			if(logActivo){
				for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
					clave->set((Registro*)bloque->get(i));
					pushCambio(Cambio(clave,nroBloque,Cambio::Alta));
				}
			}
			return true;
		}
	}
	return false;
}

bool EABloques::leer(Componente*compuesto){
	Bloque*bloque=dynamic_cast<Bloque*>(compuesto);
	if(bloque!=NULL &&nroBloque<siguienteLibre){
		return leerBloque(bloque);
	}
	return false;
}
size_t EABloques::buscarEspacioLibre(Ttamanio espacio,bool& encontrado){
	size_t nroBloque=0;
	libres=capacBloque;
	archivoEspacioLibre.seekg(0);
	encontrado=false;
	while(!encontrado && nroBloque<siguienteLibre && !archivoEspacioLibre.eof()){
		archivoEspacioLibre.read((char*)&libres,sizeof(Ttamanio));
		Ttamanio disponibles=libres-(1-porcCarga)*capacBloque;
		encontrado=disponibles>espacio and capacBloque>disponibles;
		if(!encontrado)nroBloque++;
	}
	archivoEspacioLibre.clear();
	return nroBloque;
}

bool EABloques::insertar(Componente*componente){
	bool encontrado;
	Registro*nuevo=dynamic_cast<Registro*>(componente);
	if(nuevo!=NULL){
		size_t posInsercion=buscarEspacioLibre(nuevo->tamanioSerializado(),encontrado);
		if(encontrado){
			posicionarComponente(posInsercion);
			if(!leerBloque(bloque)) return false;
			bloque->agregar(componente);
		}else{
			bloque->inicializar(componente);
			siguienteLibre++;
		}
		posicionarComponente(posInsercion);
		escribirBloque(bloque);

		if(logActivo){
			clave->set(nuevo);
			pushCambio(Cambio(clave,posInsercion,Cambio::Alta));
		}
	}
	return true;
}



bool EABloques::modificar(Componente*componente){
	size_t posModificado=nroBloque;
	/*Si el bloque que sigue es valido*/
	if(nroBloque<siguienteLibre){
		/*leo el bloque que sigue*/
		if(!leerBloque(bloque)) return false;
		Ttamanio nroComponente=0;
		/*busco el registro en el bloque leido*/
		if(buscarComponente((Registro*)componente,nroComponente)){
			Ttamanio tamanioDisponible=bloque->get(nroComponente)->tamanioSerializado()+libres;
			Ttamanio tamanioNuevo=componente->tamanioSerializado();
			if(tamanioNuevo>=capacBloque)
				return false;/*una excepcion para la proxima*/
			if(tamanioDisponible<tamanioNuevo){
				/*elimino del bloque y escribo*/
				bloque->eliminar(nroComponente);
				posicionarComponente(posModificado);
				escribirBloque(bloque);
				/* busco un bloque donde insertar*/
				bool encontrado;
				posModificado=buscarEspacioLibre(tamanioNuevo,encontrado);
				if(encontrado){
					/*existe un bloque con espacio*/
					posicionarComponente(posModificado);
					if(not leerBloque(bloque))return false;
					bloque->agregar(componente);
				}else{
					/*agrego al final*/
					bloque->inicializar(componente);
					posModificado=siguienteLibre;
					siguienteLibre++;
				}
				if(logActivo){
					clave->set((Registro*)componente);
					pushCambio(Cambio(clave,posModificado,Cambio::Reubicacion));
				}
			}else{
				/*hay espacio para la modificacion en el bloque actual*/
				Componente* eliminado=bloque->reemplazar(componente,nroComponente);
				if(eliminado!=NULL)
					delete eliminado;
			}
			/*se escribe el bloque con el registro modificado*/
			posicionarComponente(posModificado);
			escribirBloque(bloque);
			return true;
		}
	}
	return false;
}
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
}
bool EABloques::eliminar(Componente*componente){
	size_t posBorrado=nroBloque;
	if(nroBloque<siguienteLibre){
		if(not leerBloque(bloque))return false;
		Ttamanio nroComponente;
		if(buscarComponente((Registro*)componente,nroComponente)){
			if(bloque->cantidadComponentes()<=1){
				if(posBorrado<siguienteLibre-1){
					posicionarComponente(siguienteLibre-1);
					leerBloque(bloque);
					if(logActivo){
						for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
							clave->set((Registro*)bloque->get(i));
							pushCambio(Cambio(clave,nroBloque,Cambio::Reubicacion));
						}
					}
				}
				siguienteLibre--;
			}else{
				bloque->eliminar(nroComponente);
			}
			nroBloque=posBorrado;
			if(posicionarComponente(posBorrado)){
				escribirBloque(bloque);
			}
			if(logActivo){
				clave->set((Registro*)componente);
				pushCambio(Cambio(clave,nroBloque,Cambio::Baja));
			}
			return true;
		}
	}
	return false;
}
bool EABloques::siguiente(Componente*componente){
	if(nroBloque==0 and nroRegistro==0){
		leerBloque(bloque);
		nroBloque=0;
	}
	if(nroRegistro>= bloque->cantidadComponentes()){
		if(almacen->fin() or nroBloque+1>=siguienteLibre){
			return false;
		}
		if(not leerBloque(bloque))return false;
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
}
Componente *EABloques::getRegistro(){
	return bloque->get(0);
}

bool EABloques::obtener(Componente*componente){
	if(nroBloque<siguienteLibre){
		leerBloque(bloque);
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
}
size_t EABloques::posicionComponente(){
	return nroBloque;
}
bool EABloques::buscar(Componente*componente){
	bool encontrado=false;
	bool fin=false;

	posicionarComponente(0);
	while( (not fin) and not encontrado ){
		Ttamanio pos;
		size_t posComp=posicionComponente();
		fin=leer(bloque);
		if(buscarComponente((Registro*)componente,pos)){
			encontrado=true;
			std::stringbuf buf(std::ios_base::binary | std::ios_base::in | std::ios_base::out );
			buf.pubsetbuf(bloqueSerializado,capacBloque);
			buf.pubseekpos(0);

			bloque->get(pos)->serializar(buf);
			buf.pubseekpos(0);

			componente->deserializar(buf);
			if(logActivo){
				clave->set((Registro*)componente);
				pushCambio(Cambio(clave,posComp,Cambio::Reubicacion));
			}

		}
	}
	return encontrado;
}
