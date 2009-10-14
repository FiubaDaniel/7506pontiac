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
void EABloques::posicionarComponente(size_t nroCompuesto){
	corrienteBloque=nroCompuesto;
};
bool EABloques::escribir(Componente*compuesto){
	Bloque*bloque=NULL;
	if((bloque=dynamic_cast<Bloque*>(compuesto))){
		if(bloque->tamanioSerializado()<=capacBloque){
			std::stringbuf buf(std::ios_base::binary | std::ios_base::out );
			buf.pubsetbuf(bloqueSerializado,capacBloque);
			bloque->serializar(buf);
			almacen->posicionarByte(capacBloque*corrienteBloque);
			almacen->escribir(bloqueSerializado,capacBloque);
			return true;
		};
	};
	return false;
};

bool EABloques::leer(Componente*compuesto){
	Bloque*bloque=NULL;
	if((bloque=dynamic_cast<Bloque*>(compuesto))){
		std::stringbuf buf(std::ios_base::binary | std::ios_base::in );
		buf.pubsetbuf(bloqueSerializado,capacBloque);
		almacen->posicionarByte(capacBloque*corrienteBloque);
		almacen->leer(bloqueSerializado,capacBloque);
		bloque->deserializar(buf);
		return true;
	}
	return false;
};
bool EABloques::buscarEspacioLibre(Ttamanio espacio,size_t & nroBloque){
	bool encontrado=false;
	archivoEspacioLibre.seekg(std::fstream::beg);
	nroBloque=-1;
	while(!encontrado && archivoEspacioLibre.eof()&& archivoEspacioLibre.good()){
		Ttamanio disponibles;
		nroBloque++;
		archivoEspacioLibre.read((char*)&libres,sizeof(Ttamanio));
		disponibles=libres-(1-porcCarga)*capacBloque;
		if(disponibles>espacio)
			encontrado=true;
	}
	return encontrado;
};

size_t EABloques::insertar(Componente*componente){
	size_t nroBloque;
	Ttamanio  nuevoTamanio;
	if(dynamic_cast<Compuesto*>(componente)!=NULL);//TODO exepcion
	Ttamanio espacioNecesario=componente->tamanioSerializado();
	if(buscarEspacioLibre(espacioNecesario,nroBloque)){
		posicionarComponente(nroBloque);
		leer(bloque);
		bloque->agregar(componente);
	}else{
		bloque->inicializar(componente);
		nroBloque++;
	}
	nuevoTamanio=bloque->tamanioSerializado();

	archivoEspacioLibre.seekp(nroBloque*sizeof(Ttamanio));
	archivoEspacioLibre.write((char*)&nuevoTamanio,sizeof(Ttamanio));

	posicionarComponente(nroBloque);
	escribir(bloque);
	return nroBloque;
};
bool EABloques::modificar(Componente*componente){
	Ttamanio nuevoTamanio;
	archivoEspacioLibre.seekg(corrienteBloque*sizeof(Ttamanio));
	archivoEspacioLibre.read((char*)&nuevoTamanio,sizeof(Ttamanio));
	almacen->leer(bloque);
	Ttamanio nroComponente=0;
	if(!buscarComponente(componente,nroComponente))return false;
	nuevoTamanio+=bloque->get(nroComponente)->tamanioSerializado()-componente->tamanioSerializado();
	if(nuevoTamanio< capacBloque){
		Componente* eliminado=bloque->reemplazar(componente,nroComponente);
		if(eliminado!=NULL){
			escribir(bloque);
			nuevoTamanio=bloque->tamanioSerializado();
			archivoEspacioLibre.seekp(corrienteBloque*sizeof(Ttamanio));
			archivoEspacioLibre.write((char*)&nuevoTamanio,sizeof(Ttamanio));
			delete eliminado;
			return true;
		}
	}
	return false;
};
bool EABloques::buscarComponente(Componente*comp,Ttamanio & posicion){
	/*
	 * clave.set((Registro*)componente);
	 * Clave aux=clave.clonar();
	 */
	for(Ttamanio i=0;i<bloque->cantidadComponentes();i++){
		//TODO generar clave para el componente i
		/*
		 * if(comparar(clave,aux)==0){
		 *       posicion=i;
		 *       delete aux;
		 *       return true;
		 *  }
		 */
	}
	/*delete aux;*/
	return false;
};
bool EABloques::eliminar(Componente*componente){
	leer(bloque);
	Ttamanio nroComponente;
	if(!buscarComponente(componente,nroComponente)) return false;
	bloque->eliminar(nroComponente);
	Ttamanio nuevoTamanio;
	archivoEspacioLibre.seekg(corrienteBloque*sizeof(Ttamanio));
	archivoEspacioLibre.read((char*)&nuevoTamanio,sizeof(Ttamanio));
	escribir(bloque);
	nuevoTamanio=bloque->tamanioSerializado();
	archivoEspacioLibre.seekp(corrienteBloque*sizeof(Ttamanio));
	archivoEspacioLibre.write((char*)&nuevoTamanio,sizeof(Ttamanio));
	return true;
};
bool EABloques::buscar(Componente*componente){
	return false;
};
